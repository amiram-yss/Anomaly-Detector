cmake_minimum_required(VERSION 3.20)
project(Advanced_Programming)

set(CMAKE_CXX_STANDARD 14)

add_executable(Advanced_Programming anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h
        MainTrain.cpp SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h)
