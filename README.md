# 📚 Qt Library Automation System

Bu proje, **Qt framework** kullanılarak geliştirilmiş bir **Kütüphane Otomasyon Sistemidir**. Sistem; kitap, üye ve ödünç işlemlerinin yönetilmesini sağlar. Veriler, yerel bir **SQLite veritabanı** içinde saklanır.

## 🚀 Özellikler

- 📖 Kitap ekleme, düzenleme ve silme işlemleri  
- 👤 Üye kaydı ve üyelik işlemleri  
- 📅 Kitap ödünç alma ve teslim etme  
- 🧠 SQLite veritabanı ile kalıcı veri saklama  
- 🎨 Qt Designer ile kullanıcı dostu grafik arayüz

## 🛠️ Kullanılan Teknolojiler

- **C++**  
- **Qt (Widgets, Designer, UI)**  
- **SQLite (Qt SQL module)**

## 🗂️ Dosya Yapısı

├── build/ # Derleme çıktıları
├── images/ # Arayüzde kullanılan görseller
├── kitapislemleri.* # Kitap işlemleri için sınıf
├── uyeislemleri.* # Üye işlemleri sınıfı
├── oduncalmaislemleri.* # Kitap ödünç alma ekranı
├── oduncteslimislemleri.* # Kitap teslim ekranı
├── mainwindow.* # Ana arayüz
├── LibraryProject.pro # Qt proje dosyası
├── resource.qrc # Kaynak (görsel) tanımları
├── main.cpp # Giriş noktası


## 💻 Kurulum ve Çalıştırma

1. **Qt Creator** ile projeyi açın:  
   `LibraryProject.pro` dosyasını çift tıklayarak Qt Creator içinde açabilirsiniz.

2. Gerekli modüller (varsayılan olarak Qt içinde gelir):
   - `QT += core gui sql`

3. **Projeyi derleyin ve çalıştırın.**

> 📌 Veritabanı bağlantıları `QSqlDatabase` ile yapılmıştır. SQLite sürücüsü etkin olmalıdır.




