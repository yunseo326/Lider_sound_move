#include "include/CalDegree.hpp"
#include <iostream>
#include <algorithm>
#include <portaudio.h>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>

using namespace CalDegree;
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 44100*2
// 각도를 계산하는 소리의 크기 기준점
#define THRESHOLD 0.1
#define DEVICE_ID1 6
#define DEVICE_ID2 7
#define DEVICE_ID3 8
#define DEVICE_ID4 9

float inputData1[FRAMES_PER_BUFFER];
float inputData2[FRAMES_PER_BUFFER];
float inputData3[FRAMES_PER_BUFFER];
float inputData4[FRAMES_PER_BUFFER];

// 네 개의 오디오 장치에 대한 스트림 설정
PaStream *stream1, *stream2, *stream3, *stream4;
PaStreamParameters inputParameters1, inputParameters2, inputParameters3, inputParameters4;

// 첫 번째 오디오 입력을 읽는 함수 (스레드에서 실행)
void captureAudio1() {
    float buffer[FRAMES_PER_BUFFER];
    PaError err = Pa_ReadStream(stream1, buffer, FRAMES_PER_BUFFER);
    if (err != paNoError) {
        std::cerr << "Pa_ReadStream error: " << Pa_GetErrorText(err) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::copy(buffer, buffer + FRAMES_PER_BUFFER, inputData1);
}

// 두 번째 오디오 입력을 읽는 함수 (스레드에서 실행)
void captureAudio2() {
    float buffer[FRAMES_PER_BUFFER];
    PaError err = Pa_ReadStream(stream2, buffer, FRAMES_PER_BUFFER);
    if (err != paNoError) {
        std::cerr << "Pa_ReadStream error: " << Pa_GetErrorText(err) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::copy(buffer, buffer + FRAMES_PER_BUFFER, inputData2);
    }

// 세 번째 오디오 입력을 읽는 함수 (스레드에서 실행)
void captureAudio3() {
    float buffer[FRAMES_PER_BUFFER];
    PaError err = Pa_ReadStream(stream3, buffer, FRAMES_PER_BUFFER);
    if (err != paNoError) {
        std::cerr << "Pa_ReadStream error: " << Pa_GetErrorText(err) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::copy(buffer, buffer + FRAMES_PER_BUFFER, inputData3);
    }

// 네 번째 오디오 입력을 읽는 함수 (스레드에서 실행)
void captureAudio4() {
    float buffer[FRAMES_PER_BUFFER];
    PaError err = Pa_ReadStream(stream4, buffer, FRAMES_PER_BUFFER);
    if (err != paNoError) {
        std::cerr << "Pa_ReadStream error: " << Pa_GetErrorText(err) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::copy(buffer, buffer + FRAMES_PER_BUFFER, inputData4);
    }

int main(){

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio 초기화 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    
    // 첫 번째 오디오 장치 설정 (마이크 1)
    inputParameters1.device = DEVICE_ID1;  // 첫 번째 USB 마이크의 device ID
    inputParameters1.channelCount = 1;
    inputParameters1.sampleFormat = paFloat32;
    inputParameters1.suggestedLatency = Pa_GetDeviceInfo(inputParameters1.device)->defaultLowInputLatency;
    inputParameters1.hostApiSpecificStreamInfo = nullptr;

    // 두 번째 오디오 장치 설정 (마이크 2)
    inputParameters2.device = DEVICE_ID2;  // 두 번째 USB 마이크의 device ID
    inputParameters2.channelCount = 1;
    inputParameters2.sampleFormat = paFloat32;
    inputParameters2.suggestedLatency = Pa_GetDeviceInfo(inputParameters2.device)->defaultLowInputLatency;
    inputParameters2.hostApiSpecificStreamInfo = nullptr;

    // 세 번째 오디오 장치 설정 (마이크 3)
    inputParameters3.device = DEVICE_ID3;  // 세 번째 USB 마이크의 device ID
    inputParameters3.channelCount = 1;
    inputParameters3.sampleFormat = paFloat32;
    inputParameters3.suggestedLatency = Pa_GetDeviceInfo(inputParameters3.device)->defaultLowInputLatency;
    inputParameters3.hostApiSpecificStreamInfo = nullptr;

    // 네 번째 오디오 장치 설정 (마이크 4)
    inputParameters4.device = DEVICE_ID4;  // 네 번째 USB 마이크의 device ID
    inputParameters4.channelCount = 1;
    inputParameters4.sampleFormat = paFloat32;
    inputParameters4.suggestedLatency = Pa_GetDeviceInfo(inputParameters4.device)->defaultLowInputLatency;
    inputParameters4.hostApiSpecificStreamInfo = nullptr;


    // 첫 번째 스트림 열기 (마이크 1)
    err = Pa_OpenStream(&stream1,
                        &inputParameters1, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, nullptr, nullptr);
    if (err != paNoError) {
        std::cerr << "첫 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 두 번째 스트림 열기 (마이크 2)
    err = Pa_OpenStream(&stream2,
                        &inputParameters2, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, nullptr, nullptr);
    if (err != paNoError) {
        std::cerr << "두 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 세 번째 스트림 열기 (마이크 3)
    err = Pa_OpenStream(&stream3,
                        &inputParameters3, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, nullptr, nullptr);
    if (err != paNoError) {
        std::cerr << "세 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 네 번째 스트림 열기 (마이크 4)
    err = Pa_OpenStream(&stream4,
                        &inputParameters4, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, nullptr, nullptr);
    if (err != paNoError) {
        std::cerr << "네 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    // 스트림 시작 (마이크 1, 마이크 2, 마이크 3, 마이크 4)
    err = Pa_StartStream(stream1);
    if (err != paNoError) {
        std::cerr << "첫 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    err = Pa_StartStream(stream2);
    if (err != paNoError) {
        std::cerr << "두 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    err = Pa_StartStream(stream3);
    if (err != paNoError) {
        std::cerr << "세 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    err = Pa_StartStream(stream4);
    if (err != paNoError) {
        std::cerr << "네 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    std::cout << "오디오 입력을 시작합니다. 종료하려면 Ctrl+C를 누르세요..." << std::endl;
    
    while (true) {

        // 오디오 입력을 받을 스레드 실행
        std::thread captureThread1(captureAudio1);
        std::thread captureThread2(captureAudio2);
        std::thread captureThread3(captureAudio3);
        std::thread captureThread4(captureAudio4);

        // 4개의 오디오가 입력을 다 받을때까지 대기
        captureThread1.join();
        captureThread2.join();
        captureThread3.join();
        captureThread4.join();
        

        
        float* maxPtr1 = std::max_element(inputData1, inputData1+FRAMES_PER_BUFFER);
        float* maxPtr2 = std::max_element(inputData2, inputData2+FRAMES_PER_BUFFER);
        float* maxPtr3 = std::max_element(inputData3, inputData3+FRAMES_PER_BUFFER);
        float* maxPtr4 = std::max_element(inputData4, inputData4+FRAMES_PER_BUFFER);

        if (*maxPtr1 >= THRESHOLD && *maxPtr2 >= THRESHOLD && *maxPtr3 >= THRESHOLD && *maxPtr4 >= THRESHOLD){                
                std::vector<double> vecinput1(inputData1, inputData1 + FRAMES_PER_BUFFER);
                std::vector<double> vecinput2(inputData2, inputData2 + FRAMES_PER_BUFFER);
                std::vector<double> vecinput3(inputData3, inputData3 + FRAMES_PER_BUFFER);
                std::vector<double> vecinput4(inputData4, inputData4 + FRAMES_PER_BUFFER);
                    
                AudioResult result = getAudioAngle(vecinput1, vecinput2, vecinput3, vecinput4);

                std::pair<double, double> best_pair = _select_final_direction({result.angle_1, result.angle_2, result.angle_3, result.angle_4});
                //cout << "최종 방향: " << 30 << " 도" << endl;
                cout << "최종 방향: " << (best_pair.first + best_pair.second)/2 << " 도" << endl;
                cout << endl;
                return (best_pair.first + best_pair.second);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // 스트림 종료
    err = Pa_StopStream(stream1);
    if (err != paNoError) {
        std::cerr << "첫 번째 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_StopStream(stream2);
    if (err != paNoError) {
        std::cerr << "두 번째 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_StopStream(stream3);
    if (err != paNoError) {
        std::cerr << "세 번째 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_StopStream(stream4);
    if (err != paNoError) {
        std::cerr << "네 번째 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }
    Pa_Terminate();
    return 0;
}