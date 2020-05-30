package nasalogs;

import lombok.Value;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

import java.util.List;

import static java.time.LocalDateTime.now;
import static java.time.format.DateTimeFormatter.ofPattern;
import static java.util.Arrays.asList;

public class Main {

    private static final String HDFS_ROOT = "hdfs://hadoop-namenode:8020";
    public static final String INPUT_DIR = HDFS_ROOT + "/nasa/input";
    public static final String OUTPUT_DIR = HDFS_ROOT + "/nasa/output_" + now().format(ofPattern("yyyy-MM-dd_HH-mm-ss"));

    private static final List<RequestsProcessor> PROCESSORS = asList(
            new ServerErrorsAggregator(),
            new MethodStatusDateCounter(),
            new DateMethodStatusCounter(),
            new ErrorsPerWeekReporter()
    );

    public static void main(String[] args) {
        JavaSparkContext context = new JavaSparkContext(new SparkConf().setAppName("NASA Logs"));
        JavaRDD<Request> requests = getRequests(context);
        PROCESSORS.forEach(processor -> processor.process(requests, context));
        context.stop();
    }

    private static JavaRDD<Request> getRequests(JavaSparkContext context) {
        JavaRDD<ParsedRequest> parsedRequests = context.textFile(INPUT_DIR)
                .map(ParsedRequest::of);
        parsedRequests.filter(ParsedRequest::isFailure)
                .map(ParsedRequest::getEntry)
                .saveAsTextFile(OUTPUT_DIR + "/unparsed-requests");
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
