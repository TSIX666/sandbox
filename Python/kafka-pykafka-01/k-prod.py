#!/usr/bin/env python

import time

from appmetrics import metrics

from pykafka import KafkaClient

DEBUG = 0

## From AppMetrics
reservoir = metrics.histogram.UniformReservoir(1028)
hist = metrics.new_histogram("call-timer", reservoir)
raw_ns = []

client = KafkaClient(hosts="127.0.0.1:9092")

# implicitly creates the topic
#
topic = client.topics["sandbox-topic"]

# Set linger time to 100ms (default is 5s).  Default is to batch
# messages, and lingering 5s causes confusing delay in exit.
#
with topic.get_producer(linger_ms=5) as producer:
    for count in range(100000):
        msg = "m-{}".format(count)
        #print(msg)
        start_ns = time.time_ns()
        producer.produce(msg.encode('utf-8'))
        end_ns = time.time_ns()
        duration_ns = end_ns - start_ns
        raw_ns.append(duration_ns)
        hist.notify(duration_ns)

print("Call performance:")
for (k, v) in hist.get().items():
    print("  {}: {}".format(k,v))

print("Histogram:")
for v in hist.get()["histogram"]:
    if v[1] > 0:
        print("  {}".format(v))

if DEBUG:
    print("Outliers:")
    raw_ns.sort(reverse=True)
    for v in raw_ns[0:300]:
        print("  {}".format(v))
