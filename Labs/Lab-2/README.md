# ➖ Bağlı Liste Üzerinde Çıkarma İşlemi

## 📘 Proje Özeti
Bu proje, **bağlı liste (linked list)** veri yapısı kullanarak iki sayının **basamak basamak çıkarılmasını** gerçekleştiren bir C programıdır.  

Her bir sayı, bir bağlı listedeki düğümler (node) aracılığıyla temsil edilir.  
Program, iki bağlı listeyi karşılaştırarak **çıkarma işlemini (borrow mantığıyla)** uygular ve sonucu hesaplar.

---

## ⚙️ Program Akışı

### 1. Veri Hazırlığı
İki sayı, `int` dizileri olarak tanımlanır:  
```c
int digits1[] = {9,0,1,7,8};
int digits2[] = {6,2,1,5,6};
```
Burada `digits1 > digits2` olacak şekilde veriler seçilmiştir.

### 2. Bağlı Liste Oluşturma
Her dizinin elemanları, birer düğüm olarak bağlı listeye eklenir:
```c
for (i = 0; i < N; i++) {
    push(&number1, digits1[i]);
    push(&number2, digits2[i]);
}
```
`push()` fonksiyonu, yeni bir düğüm oluşturur ve listenin başına ekler.

### 3. Çıkarma İşlemi (`subtractLinkedLists`)
Fonksiyon, iki bağlı listedeki değerleri **basamak basamak** karşılaştırarak çıkarır.  
Eğer üst basamaktaki sayı küçükse, bir sonraki basamaktan **borç (borrow)** alınır:
```c
if (l1->data < l2->data) {
    borrow = 1;
    l1->data = l1->data + 10;
}
```
Her adımda fark değeri `sum` değişkenine onluk katsayıyla eklenir:
```c
sum = sum + basamakKatsayi * (l1->data - l2->data);
basamakKatsayi *= 10;
```

### 4. Sonuç Yazdırma
Tüm düğümler işlendiğinde sonuç ekrana yazdırılır:
```c
printf("Sonuc: %d\n", fark);
```

### 5. Bellek Temizleme
Tüm bağlı liste düğümleri `freeList()` fonksiyonu ile serbest bırakılır.

---

## 🧩 Temel Fonksiyonlar

| Fonksiyon | Görev |
|------------|--------|
| `createNode(int data)` | Yeni bir düğüm oluşturur ve bellekten yer ayırır. |
| `push(Node** headRef, int data)` | Yeni elemanı listenin başına ekler. |
| `subtractLinkedLists(Node* l1, Node* l2)` | İki bağlı liste arasındaki çıkarma işlemini yapar. |
| `freeList(Node* head)` | Tüm düğümleri serbest bırakır (bellek temizliği). |

---

## 💡 Önemli Detaylar

- Programda **her düğüm bir basamağı temsil eder.**  
- Sayılar **ters sırayla** saklanır, çünkü `push()` her yeni elemanı listenin başına ekler.  
- Çıkarma işlemi **borç alma (borrow)** mantığına göre yapılır.  
- Elde edilen fark, her basamak 10’un kuvvetiyle çarpılarak birleştirilir.  

---

## 🧪 Örnek Çalışma

### **Veriler:**
```
digits1 = {9,0,1,7,8}
digits2 = {6,2,1,5,6}
```

### **Adım Adım İşleyiş:**
```
Basamak farkları: (8-6), (7-5), (1-1), (0-2)+10, (9-6)
=> 2, 2, 0, 8, 3
Sonuç: 32082
```

### **Program Çıktısı:**
```
Sonuc: 32082
```

---

## 🧰 Derleme ve Çalıştırma

### Derleme
```bash
gcc GR1_Fatih_Alaybeyoglu_23011009.c -o list_subtract -Wall
```

### Çalıştırma
```bash
./list_subtract
```

---

## 🧠 Notlar
- **Lab süresi:** 30 dakika  
- Programın temel amacı, **bağlı listelerle aritmetik işlemleri modellemeyi** öğretmektir.  
- Bellek yönetimi (`malloc`, `free`) dikkatle uygulanmalıdır.  
- Eğer `digits1` küçükse (örneğin 123 - 456), sonuç negatif çıkmaz; sadece borç nedeniyle sayı kayar — bu durumda ekstra kontrol eklenebilir.

---

## 🏁 Sonuç
Bu proje, **bağlı listeler** kullanılarak **sayısal çıkarma işleminin** nasıl gerçekleştirilebileceğini göstermektedir.  
Her basamak bağımsız olarak işlenir, bu da **büyük sayıların işlenmesi (Big Integer)** gibi problemlere temel bir yaklaşımdır.  
Ayrıca proje, **dinamik bellek**, **linked list yapısı** ve **borç (borrow) mantığı** konularını pekiştirmektedir.
