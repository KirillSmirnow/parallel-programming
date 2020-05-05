package nasalogs;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaSparkContext;

import static java.util.Arrays.asList;

public class NasaLogs {

    public static void main(String[] args) {
        JavaSparkContext context = getContext();
        new NasaLogs().doSomeComputation(context);
        context.stop();
    }

    private static JavaSparkContext getContext() {
        return new JavaSparkContext(new SparkConf()
                .setAppName("NASA Logs")
        );
    }

    private void doSomeComputation(JavaSparkContext context) {
        String result = context.parallelize(asList("Tom", "Derek", "Cassandra"))
                .map(String::toUpperCase)
                .map(name -> "@" + name)
                .reduce((a, b) -> a + "_" + b);
        System.out.println(result);
    }
}
