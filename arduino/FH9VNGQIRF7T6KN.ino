/* ///////////////////////////////////////////////////////////////////////////////////
  Telescope 2 Stellarium, by Nelson Ferraz
  Created: July  2016     V1.0

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  /////////////////////////////////////////////////////////////////////////////////*/

#include "config.h"

unsigned long seg_sideral = 1003;
const double      pi = 3.14159265358979324;
volatile int lastEncoded1 = 0;
volatile long encoderValue1 = 0;
volatile int lastEncoded2 = 0;
volatile long encoderValue2 = 0;
char input[20];
char txAR[10];
char txDEC[11];
long TSL;
unsigned long t_ciclo_acumulado = 0, t_ciclo;
long Az_tel_s, Alt_tel_s;
long AR_tel_s, DEC_tel_s;
long AR_stell_s, DEC_stell_s;
double cos_phi, sin_phi;
double alt, azi;

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  pinMode(enc_1A, INPUT_PULLUP);
  pinMode(enc_1B, INPUT_PULLUP);
  pinMode(enc_2A, INPUT_PULLUP);
  pinMode(enc_2B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(enc_1A), Encoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_1B), Encoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_2A), Encoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_2B), Encoder2, CHANGE);

  cos_phi = cos((((latHH * 3600) + (latMM * 60) + latSS) / 3600.0) * pi / 180.0);
  sin_phi = sin((((latHH * 3600) + (latMM * 60) + latSS) / 3600.0) * pi / 180.0);

  TSL = poleAR_HH * 3600 + poleAR_MM * 60 + poleAR_SS + poleH_HH * 3600 + poleH_MM * 60 + poleH_SS;
  while (TSL >= 86400) TSL = TSL - 86400;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  t_ciclo = millis();
  if (t_ciclo_acumulado >= seg_sideral) {
    TSL++;
    t_ciclo_acumulado = t_ciclo_acumulado - seg_sideral;
    if (TSL >= 86400) {
      TSL = TSL - 86400;
    }
  }

  read_sensors();
  AZ_to_EQ();

  if (Serial.available() > 0) communication();

  t_ciclo = millis() - t_ciclo;
  t_ciclo_acumulado = t_ciclo_acumulado + t_ciclo;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void communication()
{
  int i = 0;
  input[i++] = Serial.read();
  delay(5);
  while ((input[i++] = Serial.read()) != '#') {
    delay(5);
  }
  input[i] = '\0';

  if (input[1] == ':' && input[2] == 'G' && input[3] == 'R' && input[4] == '#') {
    Serial.print(txAR);
  }

  if (input[1] == ':' && input[2] == 'G' && input[3] == 'D' && input[4] == '#') {
    Serial.print(txDEC);
  }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void read_sensors() {
  long h_deg, h_min, h_seg, A_deg, A_min, A_seg;

  if (encoderValue2 >= pulses_enc2 || encoderValue2 <= -pulses_enc2) {
    encoderValue2 = 0;
  }
  int enc1 = encoderValue1 / 1500;
  long encoder1_temp = encoderValue1 - (enc1 * 1500);
  long map1 = enc1 * map(1500, 0, pulses_enc1, 0, 324000);
  int enc2 = encoderValue2 / 1500;
  long encoder2_temp = encoderValue2 - (enc2 * 1500);
  long map2 = enc2 * map(1500, 0, pulses_enc2, 0, 1296000);

  Alt_tel_s = map1 + map (encoder1_temp, 0, pulses_enc1, 0, 324000);
  Az_tel_s  = map2 + map (encoder2_temp, 0, pulses_enc2, 0, 1296000);

  if (Az_tel_s < 0) Az_tel_s = 1296000 + Az_tel_s;
  if (Az_tel_s >= 1296000) Az_tel_s = Az_tel_s - 1296000 ;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Encoder1() {
  int encoded1 = (digitalRead(enc_1A) << 1) | digitalRead(enc_1B);
  int sum  = (lastEncoded1 << 2) | encoded1;
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue1 ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue1 --;
  lastEncoded1 = encoded1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Encoder2() {
  int encoded2 = (digitalRead(enc_2A) << 1) | digitalRead(enc_2B);
  int sum  = (lastEncoded2 << 2) | encoded2;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue2 ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue2 --;
  lastEncoded2 = encoded2;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void AZ_to_EQ()
{
  double delta_tel, sin_h, cos_h, sin_A, cos_A, sin_DEC, cos_DEC;
  double H_telRAD, h_telRAD, A_telRAD;
  long H_tel;
  long arHH, arMM, arSS;
  long decDEG, decMM, decSS;
  char sDEC_tel;

  A_telRAD = (Az_tel_s / 3600.0) * pi / 180.0;
  h_telRAD = (Alt_tel_s / 3600.0) * pi / 180.0;
  sin_h = sin(h_telRAD);
  cos_h = cos(h_telRAD);
  sin_A = sin(A_telRAD);
  cos_A = cos(A_telRAD);
  delta_tel = asin((sin_phi * sin_h) + (cos_phi * cos_h * cos_A));
  sin_DEC = sin(delta_tel);
  cos_DEC = cos(delta_tel);
  DEC_tel_s = long((delta_tel * 180.0 / pi) * 3600.0);

  while (DEC_tel_s >= 324000) {
    DEC_tel_s = DEC_tel_s - 324000;
  }
  while (DEC_tel_s <= -324000) {
    DEC_tel_s = DEC_tel_s + 324000;
  }

  H_telRAD = acos((sin_h - (sin_phi * sin_DEC)) / (cos_phi * cos_DEC));
  H_tel = long((H_telRAD * 180.0 / pi) * 240.0);

  if (sin_A >= 0) {
    H_tel = 86400 - H_tel;
  }
  AR_tel_s = TSL - H_tel;

  while (AR_tel_s >= 86400) {
    AR_tel_s = AR_tel_s - 86400;
  }
  while (AR_tel_s < 0) {
    AR_tel_s = AR_tel_s + 86400;
  }

  arHH = AR_tel_s / 3600;
  arMM = (AR_tel_s - arHH * 3600) / 60;
  arSS = (AR_tel_s - arHH * 3600) - arMM * 60;
  decDEG = abs(DEC_tel_s) / 3600;
  decMM = (abs(DEC_tel_s) - decDEG * 3600) / 60;
  decSS = (abs(DEC_tel_s) - decDEG * 3600) - decMM * 60;
  (DEC_tel_s < 0) ? sDEC_tel = 45 : sDEC_tel = 43;

  sprintf(txAR, "%02d:%02d:%02d#", int(arHH), int(arMM), int(arSS));
  sprintf(txDEC, "%c%02d%c%02d:%02d#", sDEC_tel, int(decDEG), 223, int(decMM), int(decSS));
}

