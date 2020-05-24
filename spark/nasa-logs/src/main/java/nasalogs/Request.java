package nasalogs;

import io.netty.handler.codec.http.HttpMethod;
import io.netty.handler.codec.http.HttpResponseStatus;
import lombok.Value;

import java.time.LocalDateTime;
import java.time.OffsetDateTime;
import java.time.ZoneOffset;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static java.lang.Integer.parseInt;
import static java.time.format.DateTimeFormatter.ofPattern;

@Value
public class Request {

    private static final Pattern ENTRY_PATTERN = Pattern.compile("(.+) - - \\[(.+)] \"(.+)\" (\\d+) (-|\\d+)");

    LocalDateTime utcDateTime;
    String host;
    HttpMethod method;
    HttpResponseStatus status;
    int responseSize;

    public static Request parse(String entry) {
        Matcher matcher = ENTRY_PATTERN.matcher(entry);
        if (!matcher.find()) {
            throw new IllegalArgumentException(String.format("Illegal entry: '%s'", entry));
        }
        LocalDateTime dateTime = parseDateTime(matcher.group(2));
        String host = matcher.group(1);
        HttpMethod method = parseMethod(matcher.group(3));
        HttpResponseStatus status = parseStatus(matcher.group(4));
        int responseSize = parseResponseSize(matcher.group(5));
        return new Request(dateTime, host, method, status, responseSize);
    }

    private static LocalDateTime parseDateTime(String dateTime) {
        return OffsetDateTime.parse(dateTime, ofPattern("dd/MMM/yyyy:HH:mm:ss Z"))
                .withOffsetSameInstant(ZoneOffset.UTC)
                .toLocalDateTime();
    }

    private static HttpMethod parseMethod(String request) {
        return HttpMethod.valueOf(request.split(" ")[0]);
    }

    private static HttpResponseStatus parseStatus(String status) {
        return HttpResponseStatus.parseLine(status);
    }

    private static int parseResponseSize(String responseSize) {
        return responseSize.equals("-") ? -1 : parseInt(responseSize);
    }
}
