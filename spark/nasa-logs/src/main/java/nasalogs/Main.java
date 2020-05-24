package nasalogs;

import lombok.Value;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

import java.util.List;

import static java.util.Arrays.asList;

public class Main {

    public static final String HDFS_ROOT = "hdfs://hadoop-namenode:8020";

    private static final List<String> LOGS = asList("/nasa/access_log_Jul95", "/nasa/access_log_Aug95");
    private static final List<RequestsProcessor> PROCESSORS = asList(new ServerErrorsAggregator());

    public static void main(String[] args) {
        JavaSparkContext context = new JavaSparkContext(new SparkConf().setAppName("NASA Logs"));
        JavaRDD<Request> requests = getRequests(context);
        PROCESSORS.forEach(processor -> processor.process(requests, context));
        context.stop();
    }

    private static JavaRDD<Request> getRequests(JavaSparkContext context) {
        JavaRDD<ParsedRequest> parsedRequests = LOGS.stream()
                .map(log -> context.textFile(HDFS_ROOT + log))
                .reduce(JavaRDD::union)
                .orElseGet(context::emptyRDD)
                .map(ParsedRequest::of);
        parsedRequests.filter(ParsedRequest::isFailure)
                .map(ParsedRequest::getEntry)
                .saveAsTextFile(HDFS_ROOT + "/nasa/report/unparsed-requests");
        return parsedRequests.filter(ParsedRequest::isSuccess)
                .map(ParsedRequest::getRequest);
    }

    @Value
    private static class ParsedRequest {

        boolean success;
        String entry;
        Request request;

        public static ParsedRequest of(String entry) {
            try {
                return new ParsedRequest(true, entry, Request.parse(entry));
            } catch (Exception e) {
                return new ParsedRequest(false, entry, null);
            }
        }

        public boolean isFailure() {
            return !success;
        }
    }
}
