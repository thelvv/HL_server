# highload_server

MyServer
```
cd cpp && docker build . -t webserver
docker run --rm -p 8080:4000 webserver
ab -n 10000 -c 10 127.0.0.1:8080/httptest/wikipedia_russia.html 

Server Software:        cppserver
Server Hostname:        127.0.0.1
Server Port:            8080

Concurrency Level:      10
Time taken for tests:   7.701 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9549800000 bytes
HTML transferred:       9548240000 bytes
Requests per second:    1298.47 [#/sec] (mean)
Time per request:       7.701 [ms] (mean)
Time per request:       0.770 [ms] (mean, across all concurrent requests)
Transfer rate:          1210948.13 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       1
Processing:     2    8   1.4      8      22
Waiting:        2    5   1.1      5      15
Total:          3    8   1.4      8      22

Percentage of the requests served within a certain time (ms)
  50%      8
  66%      8
  75%      8
  80%      9
  90%      9
  95%     10
  98%     11
  99%     12
 100%     22 (longest request)

```
Nginx sever
```
docker build . -t nginxserver -f nginx.Dockerfile
docker run --rm -p 8081:8080 nginxserver
ab -n 10000 -c 10 127.0.0.1:8081/httptest/wikipedia_russia.html 

Server Software:        nginx/1.23.3
Server Hostname:        127.0.0.1
Server Port:            8081

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954824 bytes

Concurrency Level:      10
Time taken for tests:   3.092 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9550620000 bytes
HTML transferred:       9548240000 bytes
Requests per second:    3234.41 [#/sec] (mean)
Time per request:       3.092 [ms] (mean)
Time per request:       0.309 [ms] (mean, across all concurrent requests)
Transfer rate:          3016666.08 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.0      0       1
Processing:     1    3   1.2      3      38
Waiting:        0    0   1.0      0      33
Total:          1    3   1.2      3      38

Percentage of the requests served within a certain time (ms)
  50%      3
  66%      3
  75%      3
  80%      3
  90%      4
  95%      4
  98%      5
  99%      6
 100%     38 (longest request)

```

Сравнение RPS

| Сервер \ количество воркеров  | 4       | 8       | 16      | 64      |
| ----------------------------- |  :---:  |  :---:  | :-----: | :-----: |
| cppserver                     | 999.93  | 1308.92 | 1326.02 | 1332.41 |
| nginx                         | 3065.91 | 3087.35 | 3130.87 | 3103.20 |

