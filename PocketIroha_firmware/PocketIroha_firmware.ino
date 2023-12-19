#include <M5StickCPlus.h>
#include "img/face.h"
#include "voice/dosukoi.h"
#include "voice/dosukoi0.h"
#include "voice/nande1.h"
#include "voice/gozaru.h"

// PWM出力のチャンネル
const uint8_t PWM_CHANNEL = 0;
// PWM出力の分解能
const uint8_t PWM_RESOLUTION = 8;
// PWM出力の周波数
const uint32_t PWM_FREQUENCY = getApbFrequency() / (1U << PWM_RESOLUTION);
// 音声データのサンプリング周波数(Hz)
const uint32_t SOUND_SAMPLING_RATE = 8000;

void playMusic(const uint8_t* music_data, unsigned int music_length, uint32_t sample_rate) {
    // uint32_t length = music_length;
    uint32_t length = strlen((char*)music_data);
    uint32_t delay_interval = ((uint32_t)1000000U / sample_rate);
    Serial.print("length = ");
    Serial.print(length);
    Serial.print(", delay = ");
    Serial.println(delay_interval);
    // uint32_t delay_interval = 125;
    for (int i = 0; i < length; i++) {
        ledcWrite(PWM_CHANNEL, music_data[i]);
        delayMicroseconds(delay_interval);
    }
    ledcWrite(PWM_CHANNEL, 0);
}

int state = 0;

void setup() {
    M5.begin();
    Serial.begin(9600);
    M5.Axp.ScreenBreath(100); // 画面の明るさ7〜12
    M5.Lcd.setRotation(3); // 画面を横向きにする
    M5.Lcd.fillScreen(BLACK); // 背景色
    M5.Lcd.setSwapBytes(false); // 色がおかしい場合に変更する

    // スピーカーの設定
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(26, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, 0);
}

void loop() {
    M5.update();
    int n = random(3);
    int m = random(3);
    if (n <= 1 && state != 3) {
        state = 0;
        if (m <= 0) {
            state = 2;
        }
    } else if (n <= 2 && state != 2) {
        state = 1;
        if (m <= 0) {
            state = 3;
        }
    }
    switch (state) {
        case 0:
            M5.Lcd.startWrite();
            M5.Lcd.pushImage(0, 0, imgWidth, imgHeight, dosuko_iroha0);
            M5.Lcd.endWrite();
            if (M5.BtnA.wasPressed()) playMusic(nande1_raw, nande1_raw_len, SOUND_SAMPLING_RATE);
            break;
        case 1:
            M5.Lcd.startWrite();
            M5.Lcd.pushImage(0, 0, imgWidth, imgHeight, dosuko_iroha1);
            M5.Lcd.endWrite();
            if (M5.BtnA.wasPressed()) playMusic(dosukoi0_raw, dosukoi0_raw_len, SOUND_SAMPLING_RATE);
            break;
        case 2:
            M5.Lcd.startWrite();
            M5.Lcd.pushImage(0, 0, imgWidth, imgHeight, dosuko_iroha2);
            M5.Lcd.endWrite();
            if (M5.BtnA.wasPressed()) playMusic(gozaru_raw, gozaru_raw_len, SOUND_SAMPLING_RATE);
            break;
        case 3:
            M5.Lcd.startWrite();
            M5.Lcd.pushImage(0, 0, imgWidth, imgHeight, dosuko_iroha3);
            M5.Lcd.endWrite();
            if (M5.BtnA.wasPressed()) playMusic(dosukoi_raw, dosukoi_raw_len, SOUND_SAMPLING_RATE);
            break;
    }
    delay(200);
}
