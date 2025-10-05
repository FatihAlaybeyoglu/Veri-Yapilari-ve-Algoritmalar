Key-Lock Sorting with Quick Sort

Bu proje, Böl ve Yönet (Divide and Conquer) yaklaşımıyla Quick Sort algoritmasını kullanarak, dosyadan okunan key ve lock değerlerini senkronize biçimde sıralayan bir C programıdır.

📂 Proje Dosyaları

23011009.c → Programın C kaynak kodu

23011009.pdf → Algoritma raporu ve proje açıklaması

🚀 Çalışma Mantığı

Dosyadan eleman sayısı, key değerleri ve lock değerleri okunur.

İki matris oluşturulur:

keyValues → Key indeksleri ve değerleri

lockValues → Lock indeksleri ve değerleri

Quick Sort algoritması uygulanır:

Pivot seçilir, küçük değerler sola, büyük değerler sağa alınır.

Key ve lock dizileri senkronize olarak sıralanır.

Her partition işleminde key ve lock eşleşmeleri korunur.

📥 Kurulum ve Çalıştırma
# Derleme
gcc 23011009.c -o keylock

# Çalıştırma
./keylock


Program sizden dosya adını isteyecektir. Örneğin:

Enter Filename: input.txt

✅ Girdi Formatı (input.txt)
5
12 7 19 3 25   # key değerleri
12 7 19 3 25   # lock değerleri

📤 Örnek Çıktı
1. key : 3  | 1. lock : 3
2. key : 7  | 2. lock : 7
3. key : 12 | 3. lock : 12
4. key : 19 | 4. lock : 19
5. key : 25 | 5. lock : 25

⚙️ Kullanılan Fonksiyonlar

readFile() → Dosyadan key ve lock değerlerini okur.

swapElements() → İki elemanı yer değiştirir.

findPivotIndex() → Pivotun lockValues içindeki indeksini bulur.

partition() → Quick Sort partition işlemini yapar.

quickSort() → Diziyi recursive olarak sıralar.

📌 Karşılaşılan Sorunlar

İki matrisin senkronize biçimde sıralanması gerekiyordu.

Çözüm olarak, seçilen pivot değeri her iki dizide aranarak aynı indeks üzerinden partition işlemi yapılmıştır.
