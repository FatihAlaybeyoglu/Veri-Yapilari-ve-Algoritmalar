# 🕸️ Ödül Düğümü Bulma (Graf Tabanlı Arama Algoritması)

## 📘 Proje Özeti
Bu proje, bir graf yapısında **“ödül düğümünü” (prize node)** tespit etmek için tasarlanmış bir C programıdır.  
Graf, **komşuluk listesi (adjacency list)** veri yapısı ile bellekte temsil edilir ve her düğümün bağlantıları dinamik olarak saklanır.  

Program:
- Graf verilerini bir dosyadan okur,  
- Her düğümün bağlantılarını oluşturur,  
- Yüksek dereceli (çok bağlantılı) düğümleri öncelikli sorgular,  
- Sorgu sonuçlarına göre **olasılık (isPossible)** durumlarını günceller ve  
- Nihayetinde ödül düğümünü veya olası adayları belirler.

---

## ⚙️ Program Akışı

1. **Grafın Okunması (`readGraphFromFile`)**  
   Dosyadaki her satır bir düğümün komşularını belirtir:  
   ```
   Kaynak Hedef1 Hedef2 Hedef3 ...
   ```
   Örneğin:  
   ```
   0 1 2
   1 2 3
   2 3
   3
   ```

2. **Adjacency List Oluşturma (`addEdge`)**
   Her düğümün komşuları bağlı liste (linked list) yapısında saklanır.
   ```c
   typedef struct Node {
       int vertex;
       struct Node* next;
   } Node;
   ```

3. **Düğüm Derecelerinin Hesaplanması**
   Her düğümün kaç bağlantısı olduğu hesaplanır ve sıralama dizisi oluşturulur.

4. **Sorgulama Sıralaması (`nodeOrder`)**
   Düğümler, derecelerine göre **büyükten küçüğe** sıralanır.

5. **Heuristik Arama (`findPrizeNode`)**
   Sıralanan düğümler sırayla test edilir. Komşuluk kontrolü yapılır ve
   `isPossible[]` dizisi güncellenir. Eğer sadece bir sıfır kalırsa o düğüm
   **ödül düğümü** olarak kabul edilir.

6. **Çıktı Üretimi**
   Her sorgu sonucu ekrana yazdırılır ve kalan olası adaylar belirtilir.

---

## 🧩 Temel Fonksiyonlar

| Fonksiyon | Görev |
|------------|--------|
| `createNode()` | Yeni bir bağlı liste düğümü oluşturur. |
| `addEdge()` | Grafın komşuluk listesine yeni kenar ekler. |
| `determineVertexCount()` | Dosyadaki en yüksek vertex numarasına göre düğüm sayısını belirler. |
| `readGraphFromFile()` | Graf verisini dosyadan okuyup adjacency list oluşturur. |
| `printGraph()` | Tüm grafı ekrana basar. |
| `isNeighbor()` | İki düğüm arasında doğrudan bağlantı olup olmadığını kontrol eder. |
| `findPrizeNode()` | Ödül düğümünü bulmak için sorgu ve olasılık güncelleme işlemlerini yürütür. |
| `isFinished()` | Yalnızca bir aday kalıp kalmadığını denetler. |

---

## 🧠 Algoritmanın Çalışma Mantığı
Program bir tür **eliminasyon algoritması** gibi çalışır:
1. Önce çok bağlantılı düğümler sorgulanır.
2. Her sorguda olasılık durumu yeniden güncellenir.
3. Komşuluk ilişkileri üzerinden dolaylı bağlantılar değerlendirilir.
4. Tüm olasılıklar tükendiğinde yalnızca **bir ödül düğümü** kalır.

---

## 🧪 Örnek Kullanım

### Girdi dosyası (`G1_KList.txt`)
```
0 1 2
1 2 3
2 3
3
```

### Program Çıktısı
```
0: 1 2
1: 2 3
2: 3
3:

Ödül düğümünü gir: 3

0 düğümü ödüle komşu mu? --> HAYIR
0 0 0 0
1 düğümü ödüle komşu mu? --> EVET
1 0 1 1
2 düğümü ödüle komşu mu? --> HAYIR
1 -1 1 1
3 düğümü ödüle komşu mu? --> HAYIR
1 -1 -1 1

>> 4 soruda bulundu!
>> Ödül düğüm bunlardan biri olabilir: 3
```

---

## 🧰 Derleme ve Çalıştırma

### Derleme
```bash
gcc 23011009.c -o graph_search -Wall
```

### Çalıştırma
```bash
./graph_search
```

Program çalıştığında `G1_KList.txt` dosyasını otomatik okur, kullanıcıdan ödül düğüm numarası istenir ve sonuçlar ekrana yazdırılır.


## 🏁 Sonuç
Bu proje, **graf veri yapısı**, **bağlı liste yönetimi** ve **heuristik arama** konularını bir araya getirir.  
Karmaşık bağlantı ilişkilerini modelleyip belirli bir düğümü bulmak için hem **graf teorisi** hem **özyinelemeli algoritmalar** kullanılmıştır.
