package nasalogs;

import lombok.Value;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import scala.Tuple2;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import static java.util.stream.Collectors.toList;
import static nasalogs.Main.OUTPUT_DIR;

public class MethodStatusDateCounter implements RequestsProcessor {

    @Override
    public void process(JavaRDD<Request> requests, JavaSparkContext context) {
        RequestAggregator aggregator = requests.treeAggregate(new RequestAggregator(), RequestAggregator::add, RequestAggregator::add);
        context.parallelize(aggregator.toTupleList())
                .filter(entry -> entry._2.getTotalCount() >= 10)
                .saveAsTextFile(OUTPUT_DIR + "/method-status-date-counts");
    }

    @Value
    private static class RequestAggregator implements Serializable {

        Map<AggregationKey, AggregationValue> values = new HashMap<>();

        public RequestAggregator add(Request request) {
            AggregationKey key = new AggregationKey(request);
            AggregationValue value = values.computeIfAbsent(key, $ -> new AggregationValue());
            value.add(request);
            return this;
        }

        public RequestAggregator add(RequestAggregator aggregator) {
            aggregator.values.forEach((key, value) -> this.values.merge(key, value, AggregationValue::add));
            return this;
        }

        public List<Tuple2<AggregationKey, AggregationValue>> toTupleList() {
            return values.entrySet().stream()
                    .map(entry -> new Tuple2<>(entry.getKey(), entry.getValue()))
                    .collect(toList());
        }
    }

    @Value
    private static class AggregationKey implements Serializable {

        String method;
        int status;

        public AggregationKey(Request request) {
            this.method = request.getMethod().name();
            this.status = request.getStatus().code();
        }

        @Override
        public String toString() {
            return String.format("%s/%d", method, status);
        }
    }

    @Value
    private static class AggregationValue implements Serializable {

        Map<LocalDate, Integer> counts = new TreeMap<>();

        public void add(Request request) {
            LocalDate date = request.getUtcDateTime().toLocalDate();
            counts.put(date, counts.getOrDefault(date, 0) + 1);
        }

        public AggregationValue add(AggregationValue aggregationValue) {
            aggregationValue.counts.forEach((key, value) -> this.counts.merge(key, value, Integer::sum));
            return this;
        }

        public int getTotalCount() {
            return counts.values().stream().reduce(Integer::sum).orElse(0);
        }

        @Override
        public String toString() {
            return counts.toString();
        }
    }
}
