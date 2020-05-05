package nasalogs;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaSparkContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.math.BigInteger;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

import static java.util.stream.Collectors.toList;
import static java.util.stream.IntStream.range;

public class NasaLogs {

    private static final Logger log = LoggerFactory.getLogger(NasaLogs.class);

    public static void main(String[] args) {
        JavaSparkContext context = getContext();
        doSomeComputation(context);
        context.stop();
    }

    private static JavaSparkContext getContext() {
        return new JavaSparkContext(new SparkConf()
                .setAppName("NASA Logs")
        );
    }

    private static void doSomeComputation(JavaSparkContext context) {
        log.info("Initializing");
        List<Integer> initialValues = range(0, 1000).mapToObj(i -> randomInt()).collect(toList());
        log.info("Initial values created");
        BigInteger result = context.parallelize(initialValues)
                .map(NasaLogs::process)
                .reduce(BigInteger::add);
        log.info("TOTAL result = {}", result);
    }

    private static int randomInt() {
        return ThreadLocalRandom.current().nextInt(10, 10000);
    }

    private static BigInteger process(int z) {
        log.info("Processing z = {}", z);
        BigInteger result = BigInteger.ONE;
        for (int i = 1; i < z; i++) {
            result = result.multiply(BigInteger.valueOf(i));
        }
        log.info("Processing finished z = {}, result = {}", z, result);
        return result;
    }
}
