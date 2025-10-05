# 🧮 Matris Eleman Sayısı ve Dizi Aktarımı

## 📘 Proje Özeti 
Program, kullanıcıdan alınan bir **matrisin satır ve sütun sayısına** göre dinamik olarak bellekten yer ayırır, ardından her satırda **sıfır (0) değerine kadar olan elemanları sayar** ve bu elemanları tek bir diziye aktarır.  

Amaç, **dinamik bellek yönetimi** (`malloc`, `free`) kullanarak iki boyutlu matrislerle çalışmayı ve **veri aktarımı** mantığını göstermektir.

---

## ⚙️ Program Akışı

### 1. Kullanıcıdan Girdi Alma
Program, kullanıcıdan önce satır ve sütun sayısını alır:
```text
Satır sayısını giriniz: 3  
Sütun sayısını giriniz: 4
```
Ardından matris elemanları tek tek girilir:
```text
Lütfen elemanı giriniz: 5  
Lütfen elemanı giriniz: 2  
Lütfen elemanı giriniz: 0  
Lütfen elemanı giriniz: 9  
...
```

### 2. Dinamik Matris Oluşturma
Kullanıcıdan alınan `row` ve `col` değerlerine göre matris bellekte dinamik olarak oluşturulur:
```c
int** matrix = (int**)malloc(row * sizeof(int*));
for (i = 0; i < row; i++)
    matrix[i] = (int*)malloc(col * sizeof(int));
```

### 3. Satırlardaki Eleman Sayısını Hesaplama
Her satırda sıfır (0) gelene kadar elemanlar sayılır:
```c
while (k < col && matrix[i][k] != 0) {
    satirdakiElemanSayisi++;
    k++;
}
```
Sonuçlar bir diziye (`elemanSayisi[]`) kaydedilir.

### 4. Toplam Eleman Sayısını Bulma
Tüm satırların eleman sayıları toplanır:
```c
toplamElemanSayisi += elemanSayisi[i];
```

### 5. Elemanların Yeni Diziye Aktarımı
Sıfırdan önceki tüm elemanlar `elemanlar[]` adlı yeni tek boyutlu diziye aktarılır.

### 6. Sonuçların Yazdırılması
Satırlardaki eleman sayıları ve tüm elemanlar ekrana yazdırılır:
```text
Satırlardaki eleman sayısı: 2 3 1  
Elemanlar: 5 2 8 7 6 1
```

### 7. Belleğin Serbest Bırakılması
`malloc` ile açılan her bellek alanı `free()` ile temizlenir:
```c
for (i = 0; i < row; i++)
    free(matrix[i]);
free(matrix);
free(elemanSayisi);
```

---

## 🧩 Temel Fonksiyonlar ve Değişkenler

| Değişken / Fonksiyon | Görev |
|-----------------------|--------|
| `matrix` | Kullanıcının girdiği 2B matris verilerini saklar. |
| `elemanSayisi[]` | Her satırdaki sıfırdan önceki eleman sayısını tutar. |
| `elemanlar[]` | Tüm satırlardaki geçerli elemanları tek dizide toplar. |
| `malloc` / `free` | Dinamik bellek ayırma ve temizleme işlemleri. |

---

## 🧪 Örnek Çalışma

### Girdi:
```
Satır sayısını giriniz: 3
Sütun sayısını giriniz: 4
Lütfen elemanı giriniz: 5
Lütfen elemanı giriniz: 2
Lütfen elemanı giriniz: 0
Lütfen elemanı giriniz: 8
Lütfen elemanı giriniz: 7
Lütfen elemanı giriniz: 6
Lütfen elemanı giriniz: 1
Lütfen elemanı giriniz: 0
Lütfen elemanı giriniz: 3
Lütfen elemanı giriniz: 0
Lütfen elemanı giriniz: 4
Lütfen elemanı giriniz: 5
```

### Çıktı:
```
2
3
1
6
Satırlardaki eleman sayısı: 2 3 1
Elemanlar: 5 2 8 7 6 3
```

---

## 🧰 Derleme ve Çalıştırma

### Derleme
```bash
gcc GR1_23011009.c -o matrix_count -Wall
```

### Çalıştırma
```bash
./matrix_count
```

---

## 🧠 Notlar
- Program, **0** değerini **satır sonu belirteci (delimiter)** olarak kabul eder.  
- Sıfırdan sonra gelen elemanlar sayılmaz veya aktarılmaz.  
- Dinamik bellek yönetimi sayesinde matris boyutu kullanıcıdan alınan değerlere göre değişebilir.
- **Lab süresi:** 30 dakika içinde tamamlanacak şekilde tasarlanmıştır.

---

## 🏁 Sonuç
Bu proje, **dinamik bellek yönetimi**, **çok boyutlu dizilerle çalışma** ve **veri aktarımı** gibi temel C programlama kavramlarını bir arada göstermektedir.  
Matris üzerinde yapılan sayım ve aktarım işlemleriyle, bellek verimliliği ve veri işleme tekniklerinin temeli pekiştirilmiştir.
