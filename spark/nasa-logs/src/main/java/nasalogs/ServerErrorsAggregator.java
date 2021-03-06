package nasalogs;

import io.netty.handler.codec.http.HttpStatusClass;
import lombok.extern.slf4j.Slf4j;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

import static nasalogs.Main.OUTPUT_DIR;

@Slf4j
public class ServerErrorsAggregator implements RequestsProcessor {

    @Override
    public void process(JavaRDD<Request> requests, JavaSparkContext context) {
        JavaRDD<Request> serverErrors = requests.filter(
                request -> request.getStatus().codeClass() == HttpStatusClass.SERVER_ERROR
        );
        serverErrors.saveAsTextFile(OUTPUT_DIR + "/server-errors");
        log.info("Server errors count = {}", serverErrors.count());
    }
}
