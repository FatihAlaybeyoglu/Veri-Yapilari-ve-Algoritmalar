🔐 Anahtar ve Kilit Eşleştirme (Böl ve Yönet – Quick Sort)
📘 Proje Özeti

Bu proje, Böl ve Yönet (Divide and Conquer) yaklaşımıyla Quick Sort algoritmasını kullanarak iki diziyi (anahtarlar ve kilitler) senkronize şekilde sıralamayı amaçlamaktadır.

Program, dosyadan anahtar (key) ve kilit (lock) değerlerini okuyup bu iki diziyi aynı pivot üzerinden eş zamanlı sıralayarak doğru anahtar–kilit eşleşmelerini elde eder.

⚙️ Program Akışı

Dosya Okuma (readFile)

Dosyadan önce dizinin uzunluğu, ardından anahtar ve kilit değerleri okunur.

İki matris oluşturulur:

[0][i]: Elemanın sırası (ID)

[1][i]: Elemanın değeri

Sıralama (quickSort)

partition fonksiyonu ile pivot belirlenir.

Pivot değerinden küçük elemanlar sola, büyük elemanlar sağa taşınır.

Aynı pivot değeri, hem keyValues hem lockValues dizilerinde bulunarak iki dizi senkronize biçimde sıralanır.

Ekran Çıktısı

Sıralama tamamlandıktan sonra her anahtar–kilit eşleşmesi ekrana yazdırılır.

Boş dosya veya sıfır elemanlı girişlerde uygun uyarı verilir.

🧩 Temel Fonksiyonlar
Fonksiyon	Açıklama
readFile()	Dosyadaki verileri okur ve iki matris oluşturur.
swapElements()	İki elemanın (ID ve değer) yerini değiştirir.
findPivotIndex()	Seçilen pivotun diğer dizideki indeksini bulur.
partition()	Quick Sort’un temel bölme işlemini gerçekleştirir.
quickSort()	Dizileri senkronize şekilde sıralar.
💡 Önemli Detaylar

Quick Sort algoritması her adımda diziyi pivot etrafında ikiye böler.

Pivot doğru yere yerleştirildiğinde, sol taraf küçük değerleri, sağ taraf büyük değerleri içerir.

Bu işlem rekürsif olarak devam eder; ortalama karmaşıklık O(n log n)’dir.

Eş zamanlı sıralama için findPivotIndex fonksiyonu sayesinde pivot değeri her iki dizide aynı konuma getirilir.

🧠 Karşılaşılan Sorunlar ve Çözümler

Sorun: İki matrisin senkronize şekilde sıralanması gerekiyordu.
Çözüm: İlk dizide belirlenen pivot değerinin, ikinci dizideki indeksini döndüren findPivotIndex fonksiyonu eklendi.
Böylece her partition aşamasında iki dizi de aynı pivot değerine göre işlem gördü.

🧪 Örnek Girdi / Çıktı
Girdi (input.txt):
5
3 8 2 6 5
8 2 3 5 6

Çıktı:
1. key : 2 | 1. lock : 2
2. key : 3 | 2. lock : 3
3. key : 5 | 3. lock : 5
4. key : 6 | 4. lock : 6
5. key : 8 | 5. lock : 8

🧰 Derleme ve Çalıştırma
Derleme:
gcc 23011009.c -o sort -Wall

Çalıştırma:
./sort


Program çalıştığında, kullanıcıdan dosya adı istenir:

Enter Filename: input.txt
🏁 Sonuç

Bu proje, Quick Sort algoritmasının senkronize diziler üzerinde uygulanması fikrini göstermektedir.
Hem böl ve yönet yaklaşımı hem de eş zamanlı sıralama mantığı birleştirilerek, gerçek dünyadaki “anahtar–kilit eşleştirme” problemine etkili bir çözüm sağlanmıştır.
