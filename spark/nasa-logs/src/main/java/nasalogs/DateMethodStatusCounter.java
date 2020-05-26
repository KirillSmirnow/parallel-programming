package nasalogs;

import lombok.Value;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import scala.Tuple2;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.Comparator;

import static nasalogs.Main.OUTPUT_DIR;

public class DateMethodStatusCounter implements RequestsProcessor {

    @Override
    public void process(JavaRDD<Request> requests, JavaSparkContext context) {
        requests.mapToPair(request -> new Tuple2<>(AggregationKey.of(request), 1))
                .reduceByKey(Integer::sum)
                .filter(pair -> pair._2 >= 10)
                .sortByKey()
                .saveAsTextFile(OUTPUT_DIR + "/date-method-status-counts");
    }

    @Value
    private static class AggregationKey implements Serializable, Comparable<AggregationKey> {

        LocalDate date;
        String method;
        int status;

        public static AggregationKey of(Request request) {
            return new AggregationKey(request.getUtcDateTime().toLocalDate(), request.getMethod().name(), request.getStatus().code());
        }

        @Override
        public int compareTo(AggregationKey that) {
            return Comparator.comparing(AggregationKey::getDate)
                    .thenComparing(AggregationKey::getMethod)
                    .thenComparing(AggregationKey::getStatus)
                    .compare(this, that);
        }

        @Override
        public String toString() {
            return String.format("(date=%s,method=%s,status=%s)", date, method, status);
        }
    }
}
