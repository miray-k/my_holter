#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <PulseSensorPlayground.h>  // Pulse Sensör kütüphanesi
#define TxD 2
#define RxD 3
#define USE_ARDUINO_INTERRUPTS true // kısa süreli kesinti tanımlıyoruz

//bluetooth için
SoftwareSerial mySerial(RxD, TxD);
String data; // String türünde data adında bir değişken tanımlıyoruz
// Nabız için değişkenler
const int PulseWire = 0;   // Pulse Sensörünün Mor Kablosu ANALOG PIN 0 da
int Threshold = 550;       // Hangi sinyal seviyesinden itibaren kalp atımı olarak kabul edileceğine karar vermek için bir eşik değeri tanımlıyoruz
PulseSensorPlayground pulseSensor;   // PulseSensor kütüphanesinde bulunan fonksiyonları kullanabilmek için bir nesne oluşturulur.
char Incoming_value = 0;

void setup() {
 Serial.begin(9600);
 mySerial.begin(9600); // Bluetooth için

Serial.begin(9600); // IDE monitor Tools -> Serial Monitor
 int eepromBellek = EEPROM.length();
  for (int i = 0 ; i < eepromBellek ; i++) {EEPROM.write(i, '\0');}

Serial.begin(9600); // Haberleşme baudrate hızı 9600

 // PulseSensor nesnesine değerler atanır. 
  pulseSensor.analogInput(PulseWire); 
  pulseSensor.setThreshold(Threshold); // Eşik değeri atanır. 
if (pulseSensor.begin()) {
    Serial.println("Sensör basariyla çalistirildi.");      
  } 
}

void loop() {
  Serial.println(analogRead(1));// EKG modülü için 
  delay(20);//gecikme sürecimiz
 
while(Serial.available() > 0) // bağlantı kuruluyken
{
delay(10); // 10ms zaman gecikmesi
char c = Serial.read(); // gelen veriyi karakterlere bölmek için c adında karakter tanımlıyoruz
data += c; // String türündeki data değişkenimiz karakterlerin toplamına eşitlendi
}

if(data.length() >0) // gelen verinin uzunluğu 0 dan büyükse
{
Serial.println(data); // gelen veriyi serial monitöre yaz
}
data = ""; // gelen veriyi sıfırla

  // pulseSensor nesnesindeki kalp atış hızını integer olarak döndüren fonksiyonu çağırır. Kalp atış hızı "nabiz" değişkenine aktarılır.
  int nabiz = pulseSensor.getBeatsPerMinute(); 
  // Kalp atışının gerçekleşip gerçekleşmediğini kontrol eder. Eğer sensöre gelen sinyal belirlenen eşiğin üzerinde ise kalp atışı gerçekleşmiş kabul edilir ve “true” döndürülür.
   if (pulseSensor.sawStartOfBeat()) {     
    Serial.println("Nabiz tespit edildi! "); 
    // Sensöre gelen sinyal eşik değerinin üzerinde ise ise çıktı olarak "nabiz tespit edildi" yazdırılır. 

    Serial.print("NABIZ: ");  // Output’a “NABIZ: “ yazdırılır.
    Serial.println(nabiz);    // “nabiz” değişkenindeki değer çıktıya yazdırılır.   
  } 
  delay(20);    // döngü yeniden çalıştırılmadan önce 20 milisaniye beklenir.

}
