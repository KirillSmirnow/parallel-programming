package nasalogs;

import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

public interface RequestsProcessor {

    void process(JavaRDD<Request> requests, JavaSparkContext context);
}
