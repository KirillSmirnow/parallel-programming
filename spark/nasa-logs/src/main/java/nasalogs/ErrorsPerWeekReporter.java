package nasalogs;

import lombok.AllArgsConstructor;
import lombok.Data;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Encoders;
import org.apache.spark.sql.SQLContext;
import org.apache.spark.sql.SparkSession;

import static nasalogs.Main.OUTPUT_DIR;
import static org.apache.spark.sql.functions.column;
import static org.apache.spark.sql.functions.window;

public class ErrorsPerWeekReporter implements RequestsProcessor {

    @Override
    public void process(JavaRDD<Request> requests, JavaSparkContext context) {
        SQLContext sqlContext = new SQLContext(SparkSession.builder().getOrCreate());
        Dataset<RequestBean> dataset = sqlContext.createDataset(requests.map(RequestBean::of).rdd(), Encoders.bean(RequestBean.class));
        dataset.where(column("status").startsWith("4").or(column("status").startsWith("5")))
                .groupBy(window(column("dateTime"), "1 week")).count()
                .sort("window")
                .write().json(OUTPUT_DIR + "/errors-per-week");
    }

    @Data
    @AllArgsConstructor
    public static class RequestBean {

        private String dateTime;
        private String host;
        private String method;
        private int status;
        private int responseSize;

        public static RequestBean of(Request request) {
            return new RequestBean(request.getUtcDateTime().toString(), request.getHost(),
                    request.getMethod().name(), request.getStatus().code(), request.getResponseSize());
        }
    }
}
