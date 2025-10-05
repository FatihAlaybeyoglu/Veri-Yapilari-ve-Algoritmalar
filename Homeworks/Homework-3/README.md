👥 Şirket Personel Ağacı (Tree Veri Yapısı)
📘 Proje Özeti

Bu proje, bir şirketteki personel hiyerarşisini ağaç (tree) veri yapısı kullanarak modelleyen bir C programıdır.
Her çalışan bir düğüm (node) olarak temsil edilir ve ast–üst ilişkileri, underLevel (astlar) ve sameLevel (aynı seviyedeki çalışanlar) göstericileriyle bağlantılıdır.

Program, bir dosyadan personel verilerini okuyarak şirketin ağaç yapısını oluşturur ve aşağıdaki analizleri gerçekleştirir:

Şirketin kaç seviyeden oluştuğunu hesaplar.

Her seviyede kaç çalışan bulunduğunu belirler.

Belirli bir seviyede en fazla astı olan çalışanı bulur.

Tüm çalışanların yaş ortalamasını ve toplam maaş giderini hesaplar.

⚙️ Program Akışı

Veri Okuma (readFile)

Dosyadaki personel sayısı okunur.

Her satırda şu bilgiler bulunur:

Ad Yaş Maaş PatronAdı


Patron adı "NULL" olan kişi kök (root) olarak kabul edilir.

Diğer çalışanlar, push fonksiyonu yardımıyla ilgili patronun altına eklenir.

Ağaç Yapısının Kurulması

Her çalışan TreeNode yapısıyla temsil edilir:

typedef struct TreeNode {
    char *name;
    int age;
    float salary;
    struct TreeNode *sameLevel;
    struct TreeNode *underLevel;
} TreeNode;


sameLevel: Aynı seviyedeki kardeş çalışanları gösterir.

underLevel: Bu çalışana bağlı astları gösterir.

Veri Analizi

howManyLevels() → Ağaçtaki toplam seviye (derinlik) sayısını hesaplar.

howManyEmployeeAtEachLevel() → Her seviyedeki çalışan sayısını bulur.

maxEmployeeForLevel() → Girilen seviyedeki en fazla astı olan patronu belirler.

ageAverage() → Tüm çalışanların yaş ortalamasını hesaplar.

salaryExpense() → Toplam maaş giderini hesaplar.

Bellek Yönetimi

Program sonunda tüm düğümler freeTree() ile post-order sırayla serbest bırakılır.

🧩 Temel Fonksiyonlar
Fonksiyon	Görev
createNode()	Yeni çalışan düğümü oluşturur.
findByName()	İsme göre çalışan arar.
push()	Belirtilen patronun altına yeni çalışan ekler.
howManyLevels()	Şirketin kaç seviyeden oluştuğunu bulur.
howManyEmployeeAtEachLevel()	Her seviyedeki çalışan sayısını hesaplar.
maxEmployeeForLevel()	Belirli bir seviyede en fazla astı olan kişiyi bulur.
ageAverage()	Yaş ortalamasını döndürür.
salaryExpense()	Toplam maaş giderini hesaplar.
freeTree()	Bellekteki tüm düğümleri serbest bırakır.
🧠 Programın Çalışma Mantığı

Program DFS (Derinlik Öncelikli Arama) tabanlı özyinelemeli (recursive) fonksiyonlar kullanır.

Her çalışan, hem underLevel hem sameLevel bağlantıları üzerinden gezilerek analiz edilir.

Böylece hem dikey (astlar) hem yatay (kardeşler) ilişkiler dikkate alınır.

🧮 Örnek Kullanım
Girdi dosyası (personel3.txt)
7
Ali 45 18000 NULL
Ayşe 32 12000 Ali
Mehmet 29 11000 Ali
Zeynep 28 9500 Ayşe
Ahmet 26 9000 Ayşe
Ece 25 8500 Mehmet
Murat 27 8800 Mehmet

Program Çıktısı
Ilgili sirketin personel agaci 3 seviyeden olusmaktadir
-Seviye 1: 1
-Seviye 2: 2
-Seviye 3: 4
Alt calisan sayisi en fazla olan kisiyi ogrenmek icin bir seviye giriniz: 2
-2.seviyede en fazla calisana sahip olan kisi 2 ile Ayşe dir
-Tum calisanlarin yas ortalamasi: 30.28
-Sirketin odedigi aylik personel maas gideri: 76800.00

🧰 Derleme ve Çalıştırma
Derleme:
gcc 23011009.c -o company_tree -Wall

Çalıştırma:
./company_tree


Program otomatik olarak personel3.txt dosyasını okur.
Kullanıcıdan, analiz yapılacak seviye bilgisi istenir.

🧹 Bellek Yönetimi

Tüm dinamik olarak oluşturulan düğümler ve diziler freeTree() ve free() çağrılarıyla temizlenir.
Bu sayede bellek sızıntısı (memory leak) oluşmaz.


🏁 Sonuç

Bu proje, ağaç veri yapısı kullanılarak hiyerarşik bir sistemin (örneğin bir şirket yapısının) nasıl modellenebileceğini gösterir.
Program, özyineleme (recursion), pointer yönetimi, bellek tahsisi ve veri analizi konularını bir araya getirerek hem veri yapısı hem algoritma becerilerini güçlendirir.
