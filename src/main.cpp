#include <iostream>
#include <vector>
#include <string>
#include <chrono>     // 시간 측정을 위해
#include <numeric>    // std::iota (정렬된 데이터 생성)
#include <algorithm>  // std::sort, std::reverse, std::is_sorted, std::shuffle
#include <random>     // std::mt19937, std::uniform_int_distribution
#include <functional> // std::function (함수 포인터처럼 사용)
#include <map>        // 알고리즘 이름과 함수 매핑
#include <iomanip>    // 출력 형식 지정 (소수점)
#include <fstream>    // CSV 파일 출력용
#include <cmath>      // std::ceil 등
#include <stdexcept>  // 예외 처리 


#include "sorts/bubble_sort.hpp"
#include "sorts/insertion_sort.hpp"
#include "sorts/selection_sort.hpp"
#include "sorts/merge_sort.hpp"
#include "sorts/quick_sort.hpp"
#include "sorts/heap_sort.hpp"
#include "sorts/cocktail_sort.hpp"
#include "sorts/comb_sort.hpp"
#include "sorts/tim_sort.hpp"      
#include "sorts/library_sort.hpp"
#include "sorts/tournament_sort.hpp"
#include "sorts/introsort.hpp"    



// --- 데이터 생성 함수들 ---

// 1. 정렬된 데이터 생성 (오름차순)
std::vector<int> generate_sorted(int size) {
    if (size < 0) throw std::invalid_argument("Size cannot be negative");
    std::vector<int> data(size);
    std::iota(data.begin(), data.end(), 0); // 0, 1, 2, ... size-1
    return data;
}

// 2. 역순 정렬 데이터 생성 (내림차순)
std::vector<int> generate_reversed(int size) {
     if (size < 0) throw std::invalid_argument("Size cannot be negative");
    std::vector<int> data(size);
    std::iota(data.begin(), data.end(), 0); // 0, 1, 2, ...
    std::reverse(data.begin(), data.end()); // ..., 2, 1, 0
    return data;
}

// 3. 랜덤 데이터 생성
std::vector<int> generate_random(int size) {
     if (size < 0) throw std::invalid_argument("Size cannot be negative");
    std::vector<int> data(size);
    std::iota(data.begin(), data.end(), 0); // 0, 1, 2, ...
    // 고품질 랜덤 시드 사용
    std::mt19937 g(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::shuffle(data.begin(), data.end(), g); // 무작위 섞기
    return data;
}

// 4. 부분 정렬 데이터 생성 (예: 10% 정도만 무작위)
std::vector<int> generate_partially_sorted(int size, double disorder_percentage = 0.1) {
     if (size < 0) throw std::invalid_argument("Size cannot be negative");
     if (disorder_percentage < 0.0 || disorder_percentage > 1.0) {
         disorder_percentage = 0.1; // 기본값 사용
     }
    std::vector<int> data = generate_sorted(size);
    int num_swaps = static_cast<int>(size * disorder_percentage / 2.0); // 몇 번 swap 할지 결정

    std::mt19937 g(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distrib(0, size - 1);

    for (int i = 0; i < num_swaps && size > 1; ++i) { // size > 1 조건 추가
        int idx1 = distrib(g);
        int idx2 = distrib(g);
        // 같은 인덱스가 뽑히면 다시 뽑거나 건너뛸 수 있지만, 여기선 단순 swap 진행
        if (idx1 != idx2) {
            std::swap(data[idx1], data[idx2]);
        }
    }
    return data;
}

// --- 메인 실험 함수 ---
int main() {
    // --- 실험 설정 ---
    // 프로젝트 요구사항 범위: 1K ~ 1M [cite: 18]
    std::vector<int> full_sizes = {1000, 5000, 10000,20000, 50000, 100000, 200000, 500000, 1000000}; // 전체 크기
    std::vector<int> reduced_sizes = {1000, 5000, 10000, 20000, 50000, 100000}; // O(n^2) 알고리즘용 축소된 크기
    const int REPETITIONS = 1; // 각 테스트 반복 횟수 (최소 10) 
    const double PARTIAL_DISORDER = 0.1; // 부분 정렬 데이터의 무작위 비율 (10%)

    // --- 정렬 알고리즘 맵 ---
    typedef std::function<void(std::vector<int>&)> SortFunction;
    std::map<std::string, SortFunction> algorithms;

    // 여기에 구현한 12개 정렬 함수를 맵에 추가
    algorithms["Bubble Sort"] = sorting::bubbleSort;           
    algorithms["Insertion Sort"] = sorting::insertionSort;     
    algorithms["Selection Sort"] = sorting::selectionSort;     
    algorithms["Merge Sort"] = sorting::mergeSort;             
    algorithms["Quick Sort"] = sorting::quickSort;             
    algorithms["Heap Sort"] = sorting::heapSort;             
    algorithms["Cocktail Shaker Sort"] = sorting::cocktailSort; 
    algorithms["Comb Sort"] = sorting::combSort;            
    algorithms["Tim Sort"] = sorting::timSort;                 
    algorithms["Library Sort"] = sorting::librarySort;         
    algorithms["Tournament Sort"] = sorting::tournamentSort;   
    algorithms["Introsort"] = sorting::introsort;    

    // --- 데이터 생성기 맵 ---
    // 프로젝트 요구사항: Sorted (asc/desc), Random, Reverse Sorted, Partially Sorted 
    std::map<std::string, std::function<std::vector<int>(int)>> data_generators;
    data_generators["Sorted (Asc)"] = generate_sorted;
    data_generators["Reversed (Desc)"] = generate_reversed;
    data_generators["Random"] = generate_random;
    data_generators[std::string("Partially Sorted (") + std::to_string(int(PARTIAL_DISORDER*100)) + "%)"] =
        [PARTIAL_DISORDER](int size) {
            return generate_partially_sorted(size, PARTIAL_DISORDER);
    };

    // --- 결과 출력 설정 (CSV 파일) ---
    std::ofstream results_file("sorting_results.csv");
    if (!results_file.is_open()) {
        std::cerr << "Error: Could not open results file sorting_results.csv" << std::endl;
        return 10;
    }
    // CSV 헤더 작성
    results_file << "Algorithm,DataType,Size,AvgTime(ms)\n";
    std::cout << "Starting experiments... Results will be saved to sorting_results.csv" << std::endl;
    std::cout << "Full sizes: ";
    for(int s : full_sizes) std::cout << s << " ";
    std::cout << "\nReduced sizes for O(n^2): ";
    for(int s : reduced_sizes) std::cout << s << " ";
    std::cout << "\nRepetitions per test: " << REPETITIONS << std::endl;


    // --- 실험 루프 실행 ---
    for (const auto& alg_pair : algorithms) {
        const std::string& alg_name = alg_pair.first;
        const SortFunction& sort_func = alg_pair.second;
        std::cout << "[CHECK] Algorithm registered: " << alg_name << std::endl;
        std::cout << "\nTesting Algorithm: " << alg_name << std::endl;

        // --- 알고리즘에 따라 사용할 크기 목록 결정 ---
        const std::vector<int>* sizes_to_use; // 포인터 사용
        if (alg_name == "Bubble Sort" || alg_name == "Insertion Sort" || alg_name == "Selection Sort" || alg_name == "Cocktail Shaker Sort" ||
    alg_name == "Library Sort") {
             sizes_to_use = &reduced_sizes; // 축소된 크기 목록의 주소 할당
             std::cout << "  (Note: Using reduced sizes up to " << reduced_sizes.back() << " for this O(n^2) algorithm)" << std::endl;
        } else {
             sizes_to_use = &full_sizes;    // 전체 크기 목록의 주소 할당
        }
        // --- 크기 목록 결정 끝 ---

        for (const auto& gen_pair : data_generators) {
            const std::string& data_type = gen_pair.first;
            const auto& generator_func = gen_pair.second;
            std::cout << "  Data Type: " << data_type << std::endl;

            // 포인터를 통해 올바른 크기 목록(*sizes_to_use)을 순회합니다.
            for (int size : *sizes_to_use) {
                std::cout << "    Size: " << std::setw(8) << size << " ... " << std::flush; // 출력 정렬 및 즉시 출력

                std::vector<long long> durations_ns; // 각 반복의 시간을 나노초로 저장
                bool sort_failed = false;

                try { // 데이터 생성 및 정렬 과정에서 발생할 수 있는 예외 처리
                    for (int rep = 0; rep < REPETITIONS; ++rep) {
                        // 1. 매 반복마다 새로운 데이터 생성
                        std::vector<int> data = generator_func(size);
                        // 2. 정렬할 데이터 복사본 생성
                        std::vector<int> data_copy = data;

                        // 3. 시간 측정 시작
                        auto start = std::chrono::high_resolution_clock::now();

                        // 4. 정렬 함수 호출 (복사본 사용)
                        sort_func(data_copy);

                        // 5. 시간 측정 종료
                        auto end = std::chrono::high_resolution_clock::now();

                        // 6. (권장) 정렬 결과 검증
                        if (!std::is_sorted(data_copy.begin(), data_copy.end())) {
                            std::cerr << "\n[ERROR] " << alg_name << " failed to sort " << data_type
                                    << " data (size " << size << ") on repetition " << rep + 1 << "!" << std::endl;

                            std::cerr << "[DEBUG] Dump from " << alg_name << " failure:\n";
                            for (int x : data_copy) std::cerr << x << " ";
                            std::cerr << "\n";

                            sort_failed = true;
                            break;
                        }

                        // 7. 측정된 시간 저장 (나노초)
                        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
                        durations_ns.push_back(duration.count());

                    } // End Repetitions Loop
                } catch (const std::exception& e) {
                    std::cerr << "\n[EXCEPTION] During test for " << alg_name << " on "
                              << data_type << " data (size " << size << "): " << e.what() << std::endl;
                    sort_failed = true;
                } catch (...) {
                     std::cerr << "\n[UNKNOWN EXCEPTION] During test for " << alg_name << " on "
                              << data_type << " data (size " << size << ")" << std::endl;
                     sort_failed = true;
                }


                if (sort_failed) {
                    // 정렬 실패 시 결과 기록
                     results_file << "\"" << alg_name << "\",\"" << data_type << "\"," << size << ",ERROR\n";
                     std::cout << " FAILED!" << std::endl;
                } else {
                    // 8. 평균 시간 계산 (나노초 -> 밀리초)
                    long long total_duration_ns = 0;
                    if (durations_ns.empty()) { // 반복이 0번이면
                        total_duration_ns = 0;
                    } else {
                         for (long long d : durations_ns) {
                            total_duration_ns += d;
                        }
                    }

                    // 밀리초(ms)로 변환하여 평균 계산
                    double avg_duration_ms = (REPETITIONS > 0)
                                             ? static_cast<double>(total_duration_ns) / REPETITIONS / 1e6
                                             : 0.0;

                    // 9. 결과 출력 (콘솔 & CSV 파일)
                    std::cout << "Avg Time: " << std::fixed << std::setprecision(4) << avg_duration_ms << " ms" << std::endl;
                     results_file << "\"" << alg_name << "\",\""
                                  << data_type << "\","
                                  << size << ","
                                  << std::fixed << std::setprecision(6) // CSV에는 더 높은 정밀도로 저장
                                  << avg_duration_ms << "\n";
                }
                results_file.flush(); // 각 사이즈 테스트 후 파일에 즉시 쓰기 (중단 대비)
            } // End Sizes Loop (*sizes_to_use 사용)
        } // End Data Types Loop
    } // End Algorithms Loop

    results_file.close(); // 파일 닫기
    std::cout << "\nExperiments finished. Results saved to sorting_results.csv" << std::endl;

    return 0;
}