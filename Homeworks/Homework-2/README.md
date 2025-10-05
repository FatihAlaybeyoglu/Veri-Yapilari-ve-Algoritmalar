# 🎵 Spotify Dinleme Simülasyonu

## 📘 Proje Özeti

Bu proje, **Veri Yapıları ve Algoritmalar** dersi kapsamında hazırlanmış bir **Spotify dinleme simülasyonudur**.  
Amaç, kullanıcıların ve şarkıların **bağlantılı listeler (linked list)** kullanılarak modellenmesi ve rastgele dinleme simülasyonları sonucunda **Top 10 listelerinin** oluşturulmasıdır.

Program C dilinde yazılmıştır ve temel olarak:

- Şarkıları ve kullanıcıları bellekte tutan veri yapıları oluşturur.  
- Her kullanıcı için rastgele dinleme dizileri üretir.  
- Her adımda hem kullanıcıya hem global sisteme ait dinleme sürelerini günceller.  
- Sonuçta her kullanıcı ve tüm sistem için **en çok dinlenen 10 şarkıyı** sıralar.

---

## 🧩 Temel Veri Yapıları

| Yapı | Açıklama |
|------|-----------|
| `song` | Şarkı kimliği ve süresini tutar. |
| `songList` | Tüm şarkıları çift yönlü dairesel liste şeklinde saklar. |
| `songListForUser` | Kullanıcıya ait şarkı listesi (çift yönlü dairesel liste). |
| `topTenForUser` | Kullanıcıya özel top 10 listesi (tek yönlü sıralı liste). |
| `topTenForSpotify` | Global top 10 listesi (tek yönlü sıralı liste). |
| `user` | Kullanıcı bilgileri, şarkı listesi ve top 10 listelerini içerir. |
| `spotify` | Tüm sistemi (şarkılar, kullanıcılar ve global top 10) kapsayan yapı. |

---

## ⚙️ Program Akışı

1. **Input okuma:**  
   `input5.txt` dosyasından şarkı ve kullanıcı bilgileri okunur.  
   İlk satırda şarkı ve kullanıcı sayısı bulunur, devamında her şarkının ID ve süresi yer alır.

2. **Veri yapılarının oluşturulması:**  
   - `pushSong` → Şarkıyı global havuza ekler.  
   - `pushUser` → Yeni kullanıcı ekler.  
   - `pushSongForUser` → Kullanıcıya ait şarkı listesine ekleme yapar.

3. **Dinleme simülasyonu:**  
   - `createAndListen()` fonksiyonu, kullanıcıya rastgele bir “dinleme dizisi” oluşturur.  
   - Her adımda `moveAndCalculateTimeForListenArray()` fonksiyonu çağrılır ve dinlenme süreleri güncellenir.

4. **Top Ten listelerinin oluşturulması:**  
   - Kullanıcı özelinde: `pushTopTenForUser()`  
   - Global sistemde: `pushTopTen()`  
   Her liste 10 elemanla sınırlıdır, fazlalık `popEleventhSong...()` fonksiyonlarıyla silinir.

5. **Ekran çıktısı:**  
   - Kullanıcılar  
   - Şarkı havuzu  
   - Her kullanıcının şarkı listesi  
   - Kullanıcıya özel Top Ten  
   - Global Top Ten  

---

## 🧠 Önemli Fonksiyonlar

| Fonksiyon | Görev |
|------------|--------|
| `randBetween(min, max)` | Belirtilen aralıkta rastgele sayı üretir. |
| `createSongList()` | Yeni şarkı düğümü oluşturur. |
| `createUser()` | Yeni kullanıcı oluşturur. |
| `pushTopTen()` | Top Ten listesine yeni şarkı ekler. |
| `popEleventhSongForSpotifyTopTen()` | 11. elemanı silerek listeyi 10 elemanda sınırlar. |
| `displayGlobalTopTen()` | En çok dinlenen 10 şarkıyı ekrana yazar. |

---

## 🧩 Kullanılan Veri Yapısı Özellikleri

- **Çift yönlü dairesel liste:** Şarkı ve kullanıcıya ait çalma listelerinde ileri–geri gezinti için.  
- **Tek yönlü sıralı liste:** Top Ten listelerini dinlenme süresine göre sıralı tutmak için.  
- **Dinamik bellek yönetimi:** `malloc` ve `free` kullanılarak her veri dinamik olarak oluşturulmuştur.

---

## 🐞 Karşılaşılan Sorunlar ve Çözümler

| Sorun | Açıklama | Çözüm |
|--------|-----------|--------|
| Top Ten listesi her zaman 1 elemanlı oluyordu. | Silme fonksiyonundaki döngü koşulu hatalıydı. | `count < 10` koşulu eklendi. |
| Dinleme dizisinde işaretçi güncellenmiyordu. | `moveAndCalculateTimeForListenArray` fonksiyonu `void` tanımlanmıştı. | Fonksiyon `songListForUser*` döndürecek şekilde düzenlendi. |

---

## 🧪 Örnek Girdi / Çıktı

### **Girdi (`input.txt`)**
```
6 3
S1 203
S2 253
S3 215
S4 540
S5 419
S6 300

1
3
5
6

2
3
6

3
6
```

### **Çıktı (özet):**
```
=== Kullanıcı Listesi ===
ID:3 Ad:User3
ID:2 Ad:User2
ID:1 Ad:User1
=========================
=== Global Top Ten ===
1) ID:3 Süre:7.16 dk
2) ID:5 Süre:6.98 dk
3) ID:1 Süre:6.76 dk
...
```

---

## 🧰 Derleme ve Çalıştırma

### Derleme
```bash
gcc 23011009.c -o spotify_sim -Wall
```

### Çalıştırma
```bash
./spotify_sim
```

Program, aynı dizindeki `input5.txt` dosyasını otomatik olarak okur.

---


## 🏁 Sonuç

Bu proje, **veri yapılarının gerçek dünya senaryosuna uygulanması** açısından kapsamlı bir örnektir.  
**Dairesel listeler**, **tek yönlü sıralı listeler** ve **dinamik bellek yönetimi** gibi kavramların birlikte kullanımıyla **Spotify benzeri bir dinleme ve istatistik sistemi** başarıyla simüle edilmiştir.
