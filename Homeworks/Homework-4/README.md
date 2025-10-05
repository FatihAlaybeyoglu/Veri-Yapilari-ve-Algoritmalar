🕸️ Ödül Düğümü Bulma (Graf Tabanlı Arama Algoritması)
📘 Proje Özeti

Bu proje, bir graf yapısında “ödül düğümünü” (prize node) tespit etmek için tasarlanmış bir C programıdır.
Graf, komşuluk listesi (adjacency list) veri yapısı ile bellekte temsil edilir ve her düğümün bağlantıları dinamik olarak saklanır.

Program:

Graf verilerini bir dosyadan okur,

Her düğümün bağlantılarını oluşturur,

Yüksek dereceli (çok bağlantılı) düğümleri öncelikli sorgular,

Sorgu sonuçlarına göre olasılık (isPossible) durumlarını günceller ve

Nihayetinde ödül düğümünü veya olası adayları belirler.

⚙️ Program Akışı

Grafın Okunması (readGraphFromFile)

Dosyadaki her satır bir düğümün komşularını belirtir.

Kaynak Hedef1 Hedef2 Hedef3 ...


Örneğin:

0 1 2
1 2 3
2 3
3


burada 0 düğümü 1 ve 2 ile bağlantılıdır.

Adjacency List Oluşturma (addEdge)

Her düğümün komşuları, bağlı liste (linked list) yapısında saklanır:

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;


addEdge() fonksiyonu yeni bir bağlantı (kenar) ekler.

Düğüm Derecelerinin Hesaplanması

Her düğümün kaç bağlantısı (kenarı) olduğu hesaplanır.

Bu bilgi, sorgulama sırasını belirlemede kullanılır.

Sorgulama Sıralaması (nodeOrder)

Düğümler, derecelerine göre büyükten küçüğe sıralanır.

Yüksek dereceli düğümler önce sorgulanır.

Heuristik Arama (findPrizeNode)

Sıralanan düğümler sırayla test edilir:

Her düğümün ödül düğümüne komşu olup olmadığı kontrol edilir (isNeighbor).

Eğer komşuysa, onun tüm komşularının isPossible[] değeri artırılır (olası aday olarak değerlendirilir).

Tüm düğümler birer azaltılarak normalize edilir, böylece olasılıklar dengeye getirilir.

Komşu olmayanların değerleri -1 yapılır, bu da artık elendikleri anlamına gelir.

Her adımda isFinished() fonksiyonu ile yalnızca bir tane 0 kalıp kalmadığı kontrol edilir.

Tek bir 0 kalırsa o düğüm kesin ödül düğümü olarak belirlenir.

Çıktı Üretimi

Her sorgu sonucu ekrana yazdırılır.

Sonunda ödül düğümü veya kalan olası adaylar listelenir.

🧩 Temel Fonksiyonlar
Fonksiyon	Görev
createNode()	Yeni bir bağlı liste düğümü oluşturur.
addEdge()	Grafın komşuluk listesine yeni kenar ekler.
determineVertexCount()	Dosyadaki en yüksek vertex numarasına göre düğüm sayısını belirler.
readGraphFromFile()	Graf verisini dosyadan okuyup adjacency list oluşturur.
printGraph()	Tüm grafı ekrana basar.
isNeighbor()	İki düğüm arasında doğrudan bağlantı olup olmadığını kontrol eder.
findPrizeNode()	Ödül düğümünü bulmak için sorgu ve olasılık güncelleme işlemlerini yürütür.
isFinished()	Yalnızca bir aday kalıp kalmadığını denetler.
💡 Kullanılan Veri Yapıları

Adjacency List (Komşuluk Listesi):
Grafı dinamik olarak temsil eder. Her düğümün bağlantıları bağlı liste şeklindedir.

isPossible[] Dizisi:
Her düğümün ödül adayı olma durumunu gösterir.

0: halen olası aday

>0: potansiyel olarak kuvvetli aday

-1: elenmiş düğüm

Heuristik Güncelleme:
Sorgulanan düğümün komşularına ve sonuçlara göre bu dizi sürekli güncellenir.

🧠 Algoritmanın Çalışma Mantığı

Program bir tür eliminasyon algoritması gibi çalışır:

Önce çok bağlantılı düğümler sorgulanır.

Her sorguda olasılık durumu yeniden güncellenir.

Komşuluk ilişkileri üzerinden dolaylı bağlantılar değerlendirilir.

Tüm olasılıklar tükendiğinde yalnızca bir ödül düğümü kalır.

Bu yaklaşım deterministik bir çözümden çok heuristik (yaklaşık) bir yöntemdir; yani ödül düğümünü tahmin etmeye yöneliktir.

🧪 Örnek Kullanım
Girdi dosyası (G1_KList.txt)
0 1 2
1 2 3
2 3
3

Program akışı:
0 düğümü ödüle komşu mu? --> HAYIR
1 düğümü ödüle komşu mu? --> EVET
2 düğümü ödüle komşu mu? --> HAYIR
...
>> 4 soruda bulundu!
>> Ödül düğüm bunlardan biri olabilir: 2

🧰 Derleme ve Çalıştırma
Derleme:
gcc 23011009.c -o graph_search -Wall

Çalıştırma:
./graph_search


Program çalıştığında:

G1_KList.txt dosyasını otomatik okur.

Kullanıcıdan ödül düğüm numarası istenir.

Sorgulama işlemleri ekrana adım adım yazdırılır.

🏁 Sonuç

Bu proje, graf veri yapısı, bağlı liste yönetimi, ve heuristik arama konularını bir araya getirmektedir.
Karmaşık bağlantı ilişkilerini modelleyip belirli bir düğümü bulmak için hem graf teorisi hem özyinelemeli ve sıralı algoritmaların gücü bir arada kullanılmıştır.
