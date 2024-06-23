# Tempat Sampah Pintar IoT

Proyek ini adalah sistem Tempat Sampah Pintar yang dirancang untuk memantau dan mengelola sampah secara efisien menggunakan teknologi IoT. Sistem ini mencakup sensor berat dan jarak, motor servo untuk kontrol tutup, dan integrasi dengan platform Blynk untuk pemantauan dan notifikasi secara real-time.

## Fitur

- **Pemantauan Real-Time**: Memantau jumlah sampah menggunakan sensor load cell dan ultrasonik.
- **Notifikasi Otomatis**: Mengirim notifikasi ketika tempat sampah penuh atau ketika berat melebihi batas yang ditentukan.
- **Kontrol Tutup**: Membuka dan menutup tutup secara otomatis berdasarkan level sampah dan dapat dikontrol secara manual melalui aplikasi Blynk.
- **Integrasi Data**: Terintegrasi dengan Blynk untuk visualisasi data dan kontrol real-time.

## Komponen

- **Mikrokontroler**: Wemos D1 Mini (ESP8266)
- **Sensor Berat**: Load Cell dengan modul HX711
- **Sensor Jarak**: Sensor Ultrasonik HC-SR04
- **Motor Servo**: Untuk kontrol tutup
- **Platform IoT**: Blynk

## Instalasi

1. **Clone Repository**:
    ```bash
    git clone https://github.com/Elan34W/Smart-Trash-Can-Management-Iot.git
    cd Smart-Trash-Can-Management-Iot
    ```

2. **Install Arduino IDE**:
    Download dan install Arduino IDE dari [sini](https://www.arduino.cc/en/software).

3. **Set Up Arduino IDE**:
    - Install paket board ESP8266.
    - Install library yang diperlukan: `HX711`, `Servo`, dan `Blynk`.

4. **Upload Kode**:
    - Buka `main.ino` di Arduino IDE.
    - Konfigurasikan kredensial Wi-Fi Anda dalam kode:
      ```cpp
      char ssid[] = "Your_SSID";
      char pass[] = "Your_PASSWORD";
      ```
    - Upload kode ke Wemos D1 Mini Anda.

## Penggunaan

1. **Nyalakan Sistem**: Hubungkan Wemos D1 Mini ke sumber daya.
2. **Buka Aplikasi Blynk**: Buka aplikasi Blynk di smartphone Anda dan hubungkan ke perangkat menggunakan token autentikasi yang diberikan.
3. **Pantau dan Kontrol**: Gunakan aplikasi Blynk untuk memantau status tempat sampah secara real-time dan mengontrol tutup secara manual jika diperlukan.

## Pengujian

- **Pengujian Sensor Berat**:
  - Letakkan berbagai berat di tempat sampah dan amati pembacaan pada aplikasi Blynk.
  - Verifikasi bahwa notifikasi dikirim ketika berat melebihi 500 gram.

- **Pengujian Sensor Jarak**:
  - Simulasikan level sampah yang berbeda dan verifikasi pembacaan jarak pada aplikasi Blynk.
  - Periksa notifikasi ketika jarak ke permukaan sampah 3 cm atau kurang.

- **Pengujian Motor Servo**:
  - Uji kontrol tutup otomatis dan manual menggunakan aplikasi Blynk.

## Troubleshooting

- Pastikan semua koneksi aman dan benar.
- Verifikasi kredensial Wi-Fi dan token autentikasi Blynk dimasukkan dengan benar.
- Periksa serial monitor untuk informasi debugging jika sistem tidak berfungsi sesuai harapan.
