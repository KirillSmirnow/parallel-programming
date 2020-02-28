#pragma once

int randomInt(int fromInclusive, int toExclusive) {
    return fromInclusive + rand() % (toExclusive - fromInclusive);
}
