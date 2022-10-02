/*
 * TelloGateway for KagaBitIoT
 * 
 * 2020.10.10  (FAP)m-asaoka  新規作成
 * 2020.11.03  (FAP)m-asaoka  WiFi SSID・Passwordを外部設定対応
 * 2022.07.01  (FAP)m-asaoka  emergency, stopコマンド対応
 * 2022.09.27  (FAP)m-asaoka  speed,rcコマンド対応
 *  
 */
#include <WiFi.h>
#include <WiFiUDP.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>  

#define CONSOLE Serial  // Console Serial Print

/* Tello Status */
#define TELLO_NOT_CONNECTED 0
#define TELLO_LANDED 1
#define TELLO_FLYING 2

/* micro:bit Read RX,TX */
#define RXPIN 27 
#define TXPIN 26

/* for Tello Setup */
const char CONNECT_TELLO_IP[]   = "192.168.10.1";
const char CONNECT_TELLO_SSID[] = "TELLO-D170E9";
//const char CONNECT_TELLO_SSID[] = "TELLO-FD6F23";
const char CONNECT_TELLO_PASS[] = "";
const int CONNECT_TELLO_PORT = 8889;
const int CONNECT_LOCALPORT = 8888;
                                   
/* Grove Port Init */
SoftwareSerial ss(RXPIN, TXPIN, false);
/* for WiFiUDP Read */
WiFiUDP udp;
/* Tello Status */
int telloStatus = TELLO_NOT_CONNECTED;

// SSID格納領域
String strSSID = "";
// パスワード格納領域
String strPassword = "";

/* WiFiSetup */
bool IsWiFiSetup = false;
int wifiSetUpCount = 0;

/* Serial通信セットアップ*/
void serialSetup() {
  /* micro:bit connect setup */
  CONSOLE.begin(57600);
  ss.begin(57600); //  baud rateを57600以下に設定しないと文字化け
}
/* WiFiセットアップ */
void wifiSetup() {
  // WiFi.begin(CONNECT_TELLO_SSID, CONNECT_TELLO_PASS);
  if (!IsWiFiSetup) {
    int str_len = strSSID.length() + 1;  
    char ssid[str_len];
    strSSID.toCharArray(ssid, str_len);
    str_len = strPassword.length() + 1;
    char pass[str_len];
    strPassword.toCharArray(pass, str_len);
    CONSOLE.println(ssid);
    CONSOLE.println(pass);
    WiFi.begin(ssid, pass);
    /* wait for WiFi Connect */
    while(WiFi.status() != WL_CONNECTED) {
      //CONSOLE.println("wifi connect waiting...");
      delay(500);
    }
  }
}
/* UDPセットアップ */
void udpSetup() {
  udp.begin(CONNECT_LOCALPORT);
}
/* Command送信 */
void SendCommand() {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("command");
    udp.endPacket();  
}
/* TakeOff送信 */
void SendTakeOff() {
  if (telloStatus == TELLO_LANDED) {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("takeoff");
    udp.endPacket();
    telloStatus = TELLO_FLYING;
  }
}
/* land送信 */
void SendLand() {
  if (telloStatus == TELLO_FLYING) {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("land");
    udp.endPacket();
    telloStatus = TELLO_LANDED;
  }
}
/* Stop コマンド */
void SendStop(){
   if (telloStatus == TELLO_FLYING) {
     udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
     udp.printf("stop");
     udp.endPacket();
   }
}
/* emergency コマンド */
void SendEmergency() {
   udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
   udp.printf("emergency");
   udp.endPacket();
   telloStatus = TELLO_LANDED;
}
/* Single Command送信 */
void SendSingleCommand(String strFunction){
  if (strFunction == "takeoff") {
    SendTakeOff();
    return;
  }
  if (strFunction == "land") {
    SendLand();
    return;
  }
  if (strFunction == "stop") {
    SendStop();
    return;
  }
  if (strFunction == "emergency") {
    SendEmergency();
    return;
  }
}
/* Left Command送信 */
void SendLeft(int x) {
  if (telloStatus == TELLO_FLYING) {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("left %d", x);
    udp.endPacket();
  }
}
/* Right Command送信 */
void SendRight(int x) {
  if (telloStatus == TELLO_FLYING) {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("right %d", x);
    udp.endPacket();
  }
}
/* 前進 Command送信 */
void SendForward(int y) {
  if (telloStatus == TELLO_FLYING) {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("forward %d", y);
    udp.endPacket();
  }
}
/* 後進 Command送信 */
void SendBack(int y) {
  if (telloStatus == TELLO_FLYING) {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("back %d", y);
    udp.endPacket();
  }
}
/* CW Command送信 */
void SendCW(int w) {
  if (telloStatus == TELLO_FLYING) {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("cw %d", w);
    udp.endPacket();
  }
}
/* 反時計周り Command送信 */
void SendCCW(int w) {
  if (telloStatus == TELLO_FLYING) {
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("ccw %d", w);
    udp.endPacket();
  }
}
/* UP コマンド */
void SendUP(int w){
   if (telloStatus == TELLO_FLYING) {
     udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
     udp.printf("up %d", w);
     udp.endPacket();
   }
}
/* Down コマンド */
void SendDown(int w){
   if (telloStatus == TELLO_FLYING) {
     udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
     udp.printf("down %d", w);
     udp.endPacket();
   }
}
/* WaitCommand */
void Wait(int i) {
  if (telloStatus == TELLO_FLYING) {
    delay(i);
  }
}
/* Speed コマンド */
void SetSpeed(int i) {
  if (telloStatus == TELLO_FLYING) {
     udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
     udp.printf("speed %d", i);
     udp.endPacket();
  }
}
/* Multi Command送信 */
void SendMultiCommand(String strFunction, String strValue){
  int iValue = strValue.toInt();
  if (strFunction == "left") {
    SendLeft(iValue);
    return;
  }
  if (strFunction == "right") {
    SendRight(iValue);
    return;
  }
  if (strFunction == "forward") {
    SendForward(iValue);
    return;
  }
  if (strFunction == "back") {
    SendBack(iValue);
    return;
  }
  if (strFunction == "cw") {
    SendCW(iValue);
    return;
  }
  if (strFunction == "ccw") {
    SendCCW(iValue);
    return;
  }
   if (strFunction == "up") {
    SendUP(iValue);
    return;
  }
  if (strFunction == "down") {
    SendDown(iValue);
    return;
  }
  if (strFunction == "delay") {
    Wait(iValue);
    return;
  }
  if (strFunction == "speed") {
    SetSpeed(iValue);
    return;
  }
}
/* Flipコマンド送信 */
void SendFlip(String strValue) {
   if (telloStatus == TELLO_FLYING) {
     udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
     udp.print("flip " + strValue);
     udp.endPacket();
   }
}
/* rcコマンド送信 */
void SendRC(String strValue) {
   if (telloStatus == TELLO_FLYING) {
     udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
     udp.print("rc " + strValue);
     udp.endPacket();
   }
}
/* Tello接続処理 */
void connectToTello() {
  if (telloStatus == TELLO_NOT_CONNECTED){
    udp.beginPacket(CONNECT_TELLO_IP, CONNECT_TELLO_PORT);
    udp.printf("command");
    udp.endPacket();
    // 状態を着陸状態にする
    telloStatus = TELLO_LANDED;
  }
}
/* 文字列のSplit */
int split(String data, char delimiter, String *dst){
    int index = 0;
    int arraySize = (sizeof(data)/sizeof((data)[0]));  
    int datalength = data.length();
    for (int i = 0; i < datalength; i++) {
        char tmp = data.charAt(i);
        if ( tmp == delimiter ) {
            index++;
            if ( index > (arraySize - 1)) return -1;
        }
        else dst[index] += tmp;
    }
    return (index + 1);
}
/* Telloへコマンドを送信 */
void sendCommandToTello(String strCommand) {
  // Telloへのコマンド送信準備を実行
  connectToTello();
  String cmds[2] = {"\0"}; // 分割された文字列を格納する配列 
  strCommand.trim();
  strCommand.toLowerCase();
  int iCount = split(strCommand, '=', cmds);
  if (iCount <= 1) {
    SendSingleCommand(strCommand);
    return;
  }
  if (iCount == 2) {
    String strFunction = cmds[0];
    strFunction.replace("=", "");
    // flipの場合のみ特殊コマンド
    if (strFunction == "flip") {
       SendFlip(cmds[1]);
       return;
    }
    // RCコマンドの場合も特殊コマンド
    if (strFunction == "rc") {
      SendRC(cmds[1]);
      return;
    }
    SendMultiCommand(strFunction, cmds[1]);
    return;
  }
}
/* WiFi 初期セットアップ処理 */
void SetWiFiCommand(String strCommand) {
  String cmds[2] = {"\0"}; // 分割された文字列を格納する配列 
  strCommand.trim();
  int iCount = split(strCommand, '=', cmds);
  if (iCount == 2) {
    String strFunction = cmds[0];
    strFunction.replace("=", "");
    // CONSOLE.println(strFunction);
    if (strFunction == "ss") {
      strSSID = cmds[1];
      CONSOLE.println(strSSID);
      wifiSetUpCount++;
    }
    if (strFunction == "pass") {
      strPassword = cmds[1];
      // CONSOLE.println(strPassword);
      wifiSetUpCount++;
    }
  }
}
/* 初期セットアップ */
void setup () {
  serialSetup();
  delay(5000);
  // wifiSetup();
  // udpSetup();
  // CONSOLE.println("-- HELLO (baud rate = 57600)");
}
/* ループ処理 */
void loop(){
  while (ss.available() > 0) {
    String recv_str = ss.readStringUntil('\n');
   // CONSOLE.print("Recv: ");
   // CONSOLE.println(recv_str);
   // WiFi がセットアップされるまでWaitする 
    if (!IsWiFiSetup) {
        SetWiFiCommand(recv_str);
        if (wifiSetUpCount >= 2) {
          wifiSetup();
          udpSetup();
          IsWiFiSetup = true;
          wifiSetUpCount = 0;
          break;
        }
    }
    else {
      // 送信
      sendCommandToTello(recv_str); 
    }
  }
  delay(500);
}
