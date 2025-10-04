#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
/* ----- Veri yapýlarý ----- */

typedef struct song {
    int songId;
    int time;
} song;

typedef struct songList {
    song songData;
    struct songList *next, *prev;
    int totalTime;
} songList;

typedef struct songListForUser {
    song songData;
    struct songListForUser *next, *prev;
    int totalTime;
} songListForUser;

typedef struct topTenForUser {
    song songData;
    struct topTenForUser *next;
    int totalTime;
} topTenForUser;

typedef struct topTenForSpotify {
    song songData;
    struct topTenForSpotify *next;
    int totalTime;
} topTenForSpotify;

typedef struct user {
    int userId;
    char username[50];
    songListForUser *userSongs;   // circular list
    topTenForUser   *userTopTen;  // singly-linked list
    struct user     *next;        
} user;

typedef struct spotify {
    songList        *allSongs;      // circular list
    user            *users;         // singly list
    topTenForSpotify *globalTopTen; // singly list
} spotify;

/* ----- Prototipler ----- */

int randBetween(int min, int max);

songList*           createSongList(int songTime, int songId);
songListForUser*    createSongListForUser(int songTime, int songId);
topTenForUser*      createTopTenForUser(int songTime, int songId,int totalTime);
topTenForSpotify*   createTopTenForSpotify(int songTime, int songId,int totalTime);
user*               createUser(int userId, const char *username);

void pushSong(spotify *app, int songId, int time);
void pushUser(spotify *app, int userId, const char *username);
void pushSongForUser(spotify *app, int userId, int songId, int time);

void pushTopTenForUser(user *userRef, int songId);
void popEleventhSongForUserTopTen(user *userRef);

void pushTopTen(spotify *app, int songId);
void popEleventhSongForSpotifyTopTen(spotify *app);

songList*           findSong(spotify *app, int songId);
user*               findUserById(spotify *app, int userId);

void createAndListen(spotify *app, user *userRef);
songListForUser* moveAndCalculateTimeForListenArray(spotify *app, user *userRef,songListForUser *currentSong,int offset);

void displayAllSongs(songList *head);
void displayAllUsers(user *head);
void displayUserSongs(user *head);
void displayUserTopTen(user *head);
void displayGlobalTopTen(topTenForSpotify *head);
void displayUserListenArray(const char *username, int *arr, int steps);
void readInputAndSimulate(spotify *SpotifyApp,const char *fileName);

/* ----- Yardýmcý Fonksiyonlar ----- */

/*
@brief Ýki tam sayý arasýnda rastgele bir deðer üretir.
@param min Deðer aralýðýnýn alt sýnýrý
@param max Deðer aralýðýnýn üst sýnýrý
@return min ile max arasýnda rastgele bir tamsayý
*/
int randBetween(int min, int max) {
    if (min > max) { int t = min; min = max; max = t; }
    return min + rand() % (max - min + 1);
}

/* ----- Liste Oluþturucular ----- */

/*
@brief Yeni bir songList düðümü oluþturur ve kendi kendine iþaret edecek þekilde baþlatýr.
@param songTime Düðümün þarký süresi deðeri
@param songId   Düðümün þarký kimliði
@return Oluþturulan songList düðümünün iþaretçisi
*/
songList* createSongList(int songTime, int songId) {
    songList *node = malloc(sizeof *node);
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->songData.songId = songId;
    node->songData.time   = songTime;
    node->totalTime       = 0;
    node->next = node->prev = node;
    return node;
}

/*
@brief Yeni bir songListForUser düðümü oluþturur ve kendi kendine iþaret edecek þekilde baþlatýr.
@param songTime Düðümün þarký süresi deðeri
@param songId   Düðümün þarký kimliði
@return Oluþturulan songListForUser düðümünün iþaretçisi
*/
songListForUser* createSongListForUser(int songTime, int songId) {
    songListForUser *node = malloc(sizeof *node);
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->songData.songId = songId;
    node->songData.time   = songTime;
    node->totalTime       = 0;
    node->next = node->prev = node;
    return node;
}

/*
@brief Yeni bir topTenForUser düðümü oluþturur.
@param songTime Þarký süresi deðeri
@param songId   Þarký kimliði
@param totalTime O ana kadar dinlenen toplam süre
@return Oluþturulan topTenForUser düðümünün iþaretçisi
*/
topTenForUser* createTopTenForUser(int songTime, int songId, int totalTime) {
    topTenForUser *node = malloc(sizeof *node);
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->songData.songId = songId;
    node->songData.time   = songTime;
    node->totalTime       = totalTime;
    node->next = NULL;
    return node;
}

/*
@brief Yeni bir topTenForSpotify düðümü oluþturur.
@param songTime Þarký süresi deðeri
@param songId   Þarký kimliði
@param totalTime O ana kadar dinlenen toplam süre
@return Oluþturulan topTenForSpotify düðümünün iþaretçisi
*/
topTenForSpotify* createTopTenForSpotify(int songTime, int songId, int totalTime) {
    topTenForSpotify *node = malloc(sizeof *node);
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->songData.songId = songId;
    node->songData.time   = songTime;
    node->totalTime       = totalTime;
    node->next = NULL;
    return node;
}

/*
@brief Yeni bir user nesnesi oluþturur.
@param userId   Kullanýcý kimliði
@param username Kullanýcý adý stringi
@return Oluþturulan user nesnesinin iþaretçisi
*/
user* createUser(int userId, const char *username) {
    user *node = malloc(sizeof *node);
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->userId      = userId;
    strcpy(node->username, username);
    node->userSongs   = NULL;
    node->userTopTen  = NULL;
    node->next        = NULL;
    return node;
}

/* ----- Push Fonksiyonlarý ----- */

/*
@brief Yeni bir þarkýyý global þarký havuzuna ekler (çift yönlü dairesel listeye).
@param app   Spotify uygulamasý nesnesi
@param songId Eklenecek þarký kimliði
@param time   Þarkýnýn süresi
@return
*/
void pushSong(spotify *app, int songId, int time) {
    songList *node = createSongList(time, songId);
    if (!app->allSongs) {
        app->allSongs = node;
    } else {
        songList *tail = app->allSongs->prev;
        tail->next = node;
        node->prev = tail;
        node->next = app->allSongs;
        app->allSongs->prev = node;
    }
}

/*
@brief Yeni bir kullanýcýyý uygulamaya ekler (tek yönlü listeye).
@param app      Spotify uygulamasý nesnesi
@param userId   Eklenen kullanýcý kimliði
@param username Eklenen kullanýcý adý
@return
*/
void pushUser(spotify *app, int userId, const char *username) {
    user *node = createUser(userId, username);
    node->next = app->users;
    app->users = node;
}

/*
@brief Belirli bir kullanýcý için þarký ekler (kullanýcýya ait dairesel listeye).
@param app    Spotify uygulamasý nesnesi
@param userId Þarký eklenen kullanýcý kimliði
@param songId Eklenen þarký kimliði
@param time   Þarkýnýn süresi
@return
*/
void pushSongForUser(spotify *app, int userId, int songId, int time) {
    user *currentUser = findUserById(app, userId);
    if (!currentUser) {
        printf("User Not Found: %d\n", userId);
        return;
    }
    songListForUser *node = createSongListForUser(time, songId);
    if (!currentUser->userSongs) {
        currentUser->userSongs = node;
    } else {
        songListForUser *tail = currentUser->userSongs->prev;
        tail->next = node;
        node->prev = tail;
        node->next = currentUser->userSongs;
        currentUser->userSongs->prev = node;
    }
}

/*
@brief Global Top Ten listesinin 11. elemanýný kaldýrýr (eðer varsa).
@param app Spotify uygulamasý nesnesi
@return
*/
void popEleventhSongForSpotifyTopTen(spotify *app) {
    topTenForSpotify *head = app->globalTopTen;
    if (!head) return;
    topTenForSpotify *cur = head;
    int count = 1;
    while (cur->next && count < 10) {
        cur = cur->next;
        count++;
    }
    if (cur->next) {
        topTenForSpotify *toFree = cur->next;
        cur->next = NULL;
        free(toFree);
    }
}

/*
@brief Kullanýcý Top Ten listesinin 11. elemanýný kaldýrýr (eðer varsa).
@param u Kullanýcý nesnesi
@return
*/
void popEleventhSongForUserTopTen(user *userRef) {
    topTenForUser *head = userRef->userTopTen;
    if (!head) return;
    topTenForUser *cur = head;
    int count = 1;
    while (cur->next && count < 10) {
        cur = cur->next;
        count++;
    }
    if (cur->next) {
        topTenForUser *toFree = cur->next;
        cur->next = NULL;
        free(toFree);
    }
}

/*
@brief Global Top Ten listesine yeni bir þarký ekler, süreye göre sýralý tutar.
@param app    Spotify uygulamasý nesnesi
@param songId Eklenen þarký kimliði
@return
*/
void pushTopTen(spotify *app, int songId) {
    songList *currentSong   = findSong(app, songId);
    if (!currentSong) return;
    int totalTime = currentSong->totalTime;

    topTenForSpotify *node = createTopTenForSpotify(
        currentSong->songData.time, currentSong->songData.songId, totalTime
    );
    node->next = NULL;

    if (!app->globalTopTen || totalTime >= app->globalTopTen->totalTime) {
        node->next         = app->globalTopTen;
        app->globalTopTen  = node;
    } else {
        topTenForSpotify *topTenReference = app->globalTopTen;
        while (topTenReference->next && topTenReference->next->totalTime > totalTime) {
            topTenReference = topTenReference->next;
        }
        node->next = topTenReference->next;
        topTenReference->next    = node;
    }

    popEleventhSongForSpotifyTopTen(app);
}

/*
@brief Kullanýcý Top Ten listesine yeni bir þarký ekler, süreye göre sýralý tutar.
@param u      Kullanýcý nesnesi
@param songId Eklenen þarký kimliði
@return
*/
void pushTopTenForUser(user* userRef, int songId) {
    songListForUser *currentSong = userRef->userSongs;
    while(songId != currentSong->songData.songId) {
        currentSong = currentSong->next;
    }

    int totalTime = currentSong->totalTime;
    topTenForUser *node = createTopTenForUser(
        currentSong->songData.time,
        currentSong->songData.songId,
        totalTime
    );
    node->next = NULL;
    if (!userRef->userTopTen || totalTime >= userRef->userTopTen->totalTime) {
        node->next    = userRef->userTopTen;
        userRef->userTopTen = node;
    } else {
        topTenForUser *topTenReference = userRef->userTopTen;
        while (topTenReference->next && topTenReference->next->totalTime > totalTime) {
            topTenReference = topTenReference->next;
        }
        node->next = topTenReference->next;
        topTenReference->next    = node;
    }
    popEleventhSongForUserTopTen(userRef);
}

/*
@brief Tüm þarký havuzunda verilen ID'yi arar ve ilgili düðümü döndürür.
@param app    Spotify uygulamasý nesnesi
@param songId Aranan þarký kimliði
@return Bulunan songList düðümünün iþaretçisi veya NULL
*/
songList* findSong(spotify *app, int songId) {
    if (!app->allSongs) return NULL;
    songList *currentSong = app->allSongs;
    do {
        if (currentSong->songData.songId == songId) return currentSong;
        currentSong = currentSong->next;
    } while (currentSong != app->allSongs);
    return NULL;
}

/*
@brief Kullanýcý kimliðine göre kullanýcýyý bulur.
@param app    Spotify uygulamasý nesnesi
@param userId Aranan kullanýcý kimliði
@return Bulunan user iþaretçisi veya NULL
*/
user* findUserById(spotify *app, int userId) {
    user *currentUser = app->users;
    while (currentUser) {
        if (currentUser->userId == userId) return currentUser;
        currentUser = currentUser->next;
    }
    return NULL;
}

/*
@brief Belirtilen kullanýcý için rastgele dinleme simülasyonu oluþturur.
@param app Spotify uygulamasý nesnesi
@param u   Dinleme simülasyonu yapýlacak kullanýcý
@return
*/
void createAndListen(spotify *app, user *userRef) {
    if (!userRef->userSongs) return;
    // Kullanýcýnýn þarký sayýsýný hesapla
    int count = 0,i;
    songListForUser *head = userRef->userSongs;
    do { count++; head = head->next; } while (head != userRef->userSongs);
    // Rastgele dinleme sayýsý
    int steps = randBetween(count/2, count);
    int *arr = malloc(steps * sizeof *arr);
    for (i = 0; i < steps; i++) {
        arr[i] = randBetween(-steps/2, steps/2);
    }
    displayUserListenArray(userRef->username, arr, steps);
    
    // Simülasyonda her adýmda offset kadar hareket et ve süreleri güncelle
    songListForUser *currentSong = userRef->userSongs;
    for (i = 0; i < steps; i++) {
        currentSong = moveAndCalculateTimeForListenArray(app, userRef, currentSong, arr[i]);
    }
    free(arr);
}


/*
@brief Dinleme dizisindeki bir adým offset kadar hareket ederek süreleri günceller.
@param app    Spotify uygulamasý nesnesi
@param u      Kullanýcý nesnesi
@param cur    Baþlangýç düðümü
@param offset Ýlerleme miktarý (pozitif veya negatif)
@return       Offset kadar hareket ettirildikten sonra ulaþýlan songListForUser düðümünün iþaretçisini döndürür.
*/
songListForUser* moveAndCalculateTimeForListenArray(spotify *app, user *userRef,
                                        songListForUser *currentSong,
                                        int offset) {
    songListForUser *songListForUserReference = currentSong;
    int j;
    // offset negatifse prev ile geri, pozitifse next ile ileri
    if (offset < 0) {
        for (j = 0; j < -offset; j++) songListForUserReference = songListForUserReference->prev;
    } else {
        for (j = 0; j < offset; j++)  songListForUserReference = songListForUserReference->next;
    }
    // Global toplam dinleme süresini ve kullanýcý özel toplam dinleme süresini güncelle
    songList *songListReference = findSong(app, songListForUserReference->songData.songId);
    songListReference->totalTime += songListForUserReference->songData.time;  // global toplam dinleme süresi
    songListForUserReference->totalTime  += songListForUserReference->songData.time;  // kullanýcýya ait toplam süre
    return songListForUserReference;
}

/*
@brief Genel þarký listesi bilgilerini ekrana yazdýrýr.
@param head Dairesel þarký listesi baþý
@return
*/
void displayAllSongs(songList *head) {
    if (!head) { printf("Þarký havuzu boþ.\n"); return; }
    songList *current = head;
    printf("=== Tüm Þarkýlar ===\n");
    do {
    	float duration_min = current->songData.time / 60.0f;
    	float total_min    = current->totalTime   / 60.0f;
        printf("ID:%d Süre:%2f Toplam Dinlenen Süre:%2f dk\n",
               current->songData.songId, duration_min, total_min);
        current = current->next;
    } while (current != head);
    printf("====================\n");
}

/*
@brief Kullanýcý listesini ekrana yazdýrýr.
@param head Kullanýcý listesi baþý
@return
*/
void displayAllUsers(user *head) {
    if (!head) { printf("Hiç kullanýcý yok.\n"); return; }
    printf("=== Kullanýcý Listesi ===\n");
    user *current;
    for (current = head; current; current = current->next)
        printf("ID:%d Ad:%s\n", current->userId, current->username);
    printf("=========================\n");
}

/*
@brief Bir kullanýcýnýn þarký listesini ekrana yazdýrýr.
@param u Kullanýcý nesnesi
@return
*/
void displayUserSongs(user *head) {
    if (!head->userSongs) {
        printf("%s’ýn þarký listesi boþ.\n", head->username);
        return;
    }
    printf("=== %s’ýn Þarkýlarý ===\n", head->username);
    songListForUser *current = head->userSongs;
    do {
    	float duration_min = current->songData.time / 60.0f;
    	float total_min    = current->totalTime   / 60.0f;
        printf("ID:%d Süre:%2f sn ToplamDin:%2f dk\n",
               current->songData.songId,
               duration_min,
               total_min);
        current = current->next;
    } while (current != head->userSongs);
    printf("========================\n");
}

/*
@brief Bir kullanýcýnýn Top Ten listesini ekrana yazdýrýr.
@param u Kullanýcý nesnesi
@return
*/
void displayUserTopTen(user *head) {
    if (!head->userTopTen) {
        printf("%s’ýn Top Ten boþ.\n", head->username);
        return;
    }
    printf("=== %s’ýn Top Ten ===\n", head->username);
    topTenForUser *current = head->userTopTen;
    int i = 1;
    do {
    	float total_min    = current->totalTime   / 60.0f;
        printf("%2d) ID:%d Süre:%2f dk\n",
               i++, current->songData.songId, total_min);
        current = current->next;
    } while (i <= 10 && current != NULL);
    printf("======================\n");
}

/*
@brief Global Top Ten listesini ekrana yazdýrýr.
@param head Global Top Ten listesi baþý
@return
*/
void displayGlobalTopTen(topTenForSpotify *head) {
    if (!head) {
        printf("Global Top Ten boþ.\n");
        return;
    }
    printf("=== Global Top Ten ===\n");
    topTenForSpotify *current = head;
    int i = 1;
    do {
    	float total_min    = current->totalTime   / 60.0f;
        printf("%2d) ID:%d Süre:%2f dk\n",
               i++, current->songData.songId, total_min);
        current = current->next;
    } while (current != NULL && i <= 10);
    printf("======================\n");
}

/*
@brief Kullanýcýnýn dinleme dizisini ekrana yazdýrýr.
@param username Kullanýcý adý
@param arr Dinleme adýmlarýný içeren dizi
@param steps Adým sayýsý
@return
*/
void displayUserListenArray(const char *username, int *arr, int steps) {
    if (!arr || steps <= 0) return;
    printf("=== %s’ýn Dinleme Dizisi (%d adým) ===\n", username, steps);
    int i;
    for (i = 0; i < steps; i++) {
        printf("%3d: %d\n", i+1, arr[i]);
    }
    printf("===================================\n");
}
/*
 @brief input.txt’ten þarkýlarý ve kullanýcý-þarký atamalarýný okur.
 @param fileName dosya ismi
 @return
*/
void readInputAndSimulate(spotify *SpotifyApp,const char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (!f) { perror("fopen"); exit(EXIT_FAILURE); }

    int songCount, userCount;
    if (fscanf(f, "%d %d\n", &songCount, &userCount) != 2) {
        fprintf(stderr, "Invalid header line\n");
        exit(EXIT_FAILURE);
    }
	int i,uid;
    // 1) þarkýlarý oku
    for (i = 0; i < songCount; i++) {
        char sid[16];
        int seconds;
        if (fscanf(f, "%15s %d\n", sid, &seconds) != 2) {
            fprintf(stderr, "Invalid song line %d\n", i+1);
            exit(EXIT_FAILURE);
        }
        // sid = "S<number>" 
        int id = atoi(sid + 1);
        pushSong(SpotifyApp, id, seconds);
    }

    // 2) kullanýcýlarý ve þarký atamalarýný oku
    for (uid = 1; uid <= userCount; uid++) {
        char line[64];
        // kullanýcýyý ekle “User#” 
        char uname[16];
        snprintf(uname, sizeof uname, "User%d", uid);
        pushUser(SpotifyApp, uid, uname);

        // blok sonuna kadar satýr satýr oku
        while (fgets(line, sizeof line, f)) {
            // boþ satýrsa bir sonraki kullanýcýya geç
            if (line[0] == '\n' || line[0] == '\r') break;
            // satýr içindeki ilk tamsayý þarký id
            char *p = line;
            // baþýnda boþluk varsa atla
            while (*p && isspace((unsigned char)*p)) p++;
            int songId = atoi(p);
            if (songId > 0)
                pushSongForUser(SpotifyApp, uid, songId,
                                findSong(SpotifyApp, songId)->songData.time);
        }
    }
	for (uid = 1; uid <= userCount; uid++) {
        user *userRef = findUserById(SpotifyApp, uid);
        createAndListen(SpotifyApp, userRef);
    }
    user *userRef;
    SpotifyApp->globalTopTen = NULL;
    for (userRef = SpotifyApp->users; userRef; userRef = userRef->next) {
        userRef->userTopTen = NULL;
    }
for (userRef = SpotifyApp->users; userRef; userRef = userRef->next) {
    if (userRef->userSongs) {                     // boþ liste kontrolü
        songListForUser *songListRef = userRef->userSongs;
        do {
            pushTopTenForUser(userRef, songListRef->songData.songId);
            songListRef = songListRef->next;
        } while (songListRef != userRef->userSongs);
    }
}
if (SpotifyApp->allSongs) {
    songList *songRef = SpotifyApp->allSongs;
    do {
        pushTopTen(SpotifyApp, songRef->songData.songId);
        songRef = songRef->next;
    } while (songRef != SpotifyApp->allSongs);
}
    fclose(f);
}

/* ----- main ----- */

/*
@brief Uygulamanýn baþlangýç noktasý, kullanýcý ve þarký sayýsýný alýr, verileri oluþturur ve simülasyonu baþlatýr.
@return Uygulama baþarýyla tamamlandýðýnda 0 döndürür.
*/
int main(void) {
	spotify SpotifyApp = { NULL, NULL, NULL };
    srand((unsigned)time(NULL));
    setlocale(LC_ALL, "");
    readInputAndSimulate(&SpotifyApp,"input5.txt");
    displayAllUsers(SpotifyApp.users);
    displayAllSongs(SpotifyApp.allSongs);
    user *u;
    for (u = SpotifyApp.users; u; u = u->next) {
        displayUserSongs(u);
        displayUserTopTen(u);
    }
    displayGlobalTopTen(SpotifyApp.globalTopTen);
    return 0;
}

