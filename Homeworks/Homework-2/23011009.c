#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
/* ----- Veri yap�lar� ----- */

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

/* ----- Yard�mc� Fonksiyonlar ----- */

/*
@brief �ki tam say� aras�nda rastgele bir de�er �retir.
@param min De�er aral���n�n alt s�n�r�
@param max De�er aral���n�n �st s�n�r�
@return min ile max aras�nda rastgele bir tamsay�
*/
int randBetween(int min, int max) {
    if (min > max) { int t = min; min = max; max = t; }
    return min + rand() % (max - min + 1);
}

/* ----- Liste Olu�turucular ----- */

/*
@brief Yeni bir songList d���m� olu�turur ve kendi kendine i�aret edecek �ekilde ba�lat�r.
@param songTime D���m�n �ark� s�resi de�eri
@param songId   D���m�n �ark� kimli�i
@return Olu�turulan songList d���m�n�n i�aret�isi
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
@brief Yeni bir songListForUser d���m� olu�turur ve kendi kendine i�aret edecek �ekilde ba�lat�r.
@param songTime D���m�n �ark� s�resi de�eri
@param songId   D���m�n �ark� kimli�i
@return Olu�turulan songListForUser d���m�n�n i�aret�isi
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
@brief Yeni bir topTenForUser d���m� olu�turur.
@param songTime �ark� s�resi de�eri
@param songId   �ark� kimli�i
@param totalTime O ana kadar dinlenen toplam s�re
@return Olu�turulan topTenForUser d���m�n�n i�aret�isi
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
@brief Yeni bir topTenForSpotify d���m� olu�turur.
@param songTime �ark� s�resi de�eri
@param songId   �ark� kimli�i
@param totalTime O ana kadar dinlenen toplam s�re
@return Olu�turulan topTenForSpotify d���m�n�n i�aret�isi
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
@brief Yeni bir user nesnesi olu�turur.
@param userId   Kullan�c� kimli�i
@param username Kullan�c� ad� stringi
@return Olu�turulan user nesnesinin i�aret�isi
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

/* ----- Push Fonksiyonlar� ----- */

/*
@brief Yeni bir �ark�y� global �ark� havuzuna ekler (�ift y�nl� dairesel listeye).
@param app   Spotify uygulamas� nesnesi
@param songId Eklenecek �ark� kimli�i
@param time   �ark�n�n s�resi
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
@brief Yeni bir kullan�c�y� uygulamaya ekler (tek y�nl� listeye).
@param app      Spotify uygulamas� nesnesi
@param userId   Eklenen kullan�c� kimli�i
@param username Eklenen kullan�c� ad�
@return
*/
void pushUser(spotify *app, int userId, const char *username) {
    user *node = createUser(userId, username);
    node->next = app->users;
    app->users = node;
}

/*
@brief Belirli bir kullan�c� i�in �ark� ekler (kullan�c�ya ait dairesel listeye).
@param app    Spotify uygulamas� nesnesi
@param userId �ark� eklenen kullan�c� kimli�i
@param songId Eklenen �ark� kimli�i
@param time   �ark�n�n s�resi
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
@brief Global Top Ten listesinin 11. eleman�n� kald�r�r (e�er varsa).
@param app Spotify uygulamas� nesnesi
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
@brief Kullan�c� Top Ten listesinin 11. eleman�n� kald�r�r (e�er varsa).
@param u Kullan�c� nesnesi
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
@brief Global Top Ten listesine yeni bir �ark� ekler, s�reye g�re s�ral� tutar.
@param app    Spotify uygulamas� nesnesi
@param songId Eklenen �ark� kimli�i
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
@brief Kullan�c� Top Ten listesine yeni bir �ark� ekler, s�reye g�re s�ral� tutar.
@param u      Kullan�c� nesnesi
@param songId Eklenen �ark� kimli�i
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
@brief T�m �ark� havuzunda verilen ID'yi arar ve ilgili d���m� d�nd�r�r.
@param app    Spotify uygulamas� nesnesi
@param songId Aranan �ark� kimli�i
@return Bulunan songList d���m�n�n i�aret�isi veya NULL
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
@brief Kullan�c� kimli�ine g�re kullan�c�y� bulur.
@param app    Spotify uygulamas� nesnesi
@param userId Aranan kullan�c� kimli�i
@return Bulunan user i�aret�isi veya NULL
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
@brief Belirtilen kullan�c� i�in rastgele dinleme sim�lasyonu olu�turur.
@param app Spotify uygulamas� nesnesi
@param u   Dinleme sim�lasyonu yap�lacak kullan�c�
@return
*/
void createAndListen(spotify *app, user *userRef) {
    if (!userRef->userSongs) return;
    // Kullan�c�n�n �ark� say�s�n� hesapla
    int count = 0,i;
    songListForUser *head = userRef->userSongs;
    do { count++; head = head->next; } while (head != userRef->userSongs);
    // Rastgele dinleme say�s�
    int steps = randBetween(count/2, count);
    int *arr = malloc(steps * sizeof *arr);
    for (i = 0; i < steps; i++) {
        arr[i] = randBetween(-steps/2, steps/2);
    }
    displayUserListenArray(userRef->username, arr, steps);
    
    // Sim�lasyonda her ad�mda offset kadar hareket et ve s�releri g�ncelle
    songListForUser *currentSong = userRef->userSongs;
    for (i = 0; i < steps; i++) {
        currentSong = moveAndCalculateTimeForListenArray(app, userRef, currentSong, arr[i]);
    }
    free(arr);
}


/*
@brief Dinleme dizisindeki bir ad�m offset kadar hareket ederek s�releri g�nceller.
@param app    Spotify uygulamas� nesnesi
@param u      Kullan�c� nesnesi
@param cur    Ba�lang�� d���m�
@param offset �lerleme miktar� (pozitif veya negatif)
@return       Offset kadar hareket ettirildikten sonra ula��lan songListForUser d���m�n�n i�aret�isini d�nd�r�r.
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
    // Global toplam dinleme s�resini ve kullan�c� �zel toplam dinleme s�resini g�ncelle
    songList *songListReference = findSong(app, songListForUserReference->songData.songId);
    songListReference->totalTime += songListForUserReference->songData.time;  // global toplam dinleme s�resi
    songListForUserReference->totalTime  += songListForUserReference->songData.time;  // kullan�c�ya ait toplam s�re
    return songListForUserReference;
}

/*
@brief Genel �ark� listesi bilgilerini ekrana yazd�r�r.
@param head Dairesel �ark� listesi ba��
@return
*/
void displayAllSongs(songList *head) {
    if (!head) { printf("�ark� havuzu bo�.\n"); return; }
    songList *current = head;
    printf("=== T�m �ark�lar ===\n");
    do {
    	float duration_min = current->songData.time / 60.0f;
    	float total_min    = current->totalTime   / 60.0f;
        printf("ID:%d S�re:%2f Toplam Dinlenen S�re:%2f dk\n",
               current->songData.songId, duration_min, total_min);
        current = current->next;
    } while (current != head);
    printf("====================\n");
}

/*
@brief Kullan�c� listesini ekrana yazd�r�r.
@param head Kullan�c� listesi ba��
@return
*/
void displayAllUsers(user *head) {
    if (!head) { printf("Hi� kullan�c� yok.\n"); return; }
    printf("=== Kullan�c� Listesi ===\n");
    user *current;
    for (current = head; current; current = current->next)
        printf("ID:%d Ad:%s\n", current->userId, current->username);
    printf("=========================\n");
}

/*
@brief Bir kullan�c�n�n �ark� listesini ekrana yazd�r�r.
@param u Kullan�c� nesnesi
@return
*/
void displayUserSongs(user *head) {
    if (!head->userSongs) {
        printf("%s��n �ark� listesi bo�.\n", head->username);
        return;
    }
    printf("=== %s��n �ark�lar� ===\n", head->username);
    songListForUser *current = head->userSongs;
    do {
    	float duration_min = current->songData.time / 60.0f;
    	float total_min    = current->totalTime   / 60.0f;
        printf("ID:%d S�re:%2f sn ToplamDin:%2f dk\n",
               current->songData.songId,
               duration_min,
               total_min);
        current = current->next;
    } while (current != head->userSongs);
    printf("========================\n");
}

/*
@brief Bir kullan�c�n�n Top Ten listesini ekrana yazd�r�r.
@param u Kullan�c� nesnesi
@return
*/
void displayUserTopTen(user *head) {
    if (!head->userTopTen) {
        printf("%s��n Top Ten bo�.\n", head->username);
        return;
    }
    printf("=== %s��n Top Ten ===\n", head->username);
    topTenForUser *current = head->userTopTen;
    int i = 1;
    do {
    	float total_min    = current->totalTime   / 60.0f;
        printf("%2d) ID:%d S�re:%2f dk\n",
               i++, current->songData.songId, total_min);
        current = current->next;
    } while (i <= 10 && current != NULL);
    printf("======================\n");
}

/*
@brief Global Top Ten listesini ekrana yazd�r�r.
@param head Global Top Ten listesi ba��
@return
*/
void displayGlobalTopTen(topTenForSpotify *head) {
    if (!head) {
        printf("Global Top Ten bo�.\n");
        return;
    }
    printf("=== Global Top Ten ===\n");
    topTenForSpotify *current = head;
    int i = 1;
    do {
    	float total_min    = current->totalTime   / 60.0f;
        printf("%2d) ID:%d S�re:%2f dk\n",
               i++, current->songData.songId, total_min);
        current = current->next;
    } while (current != NULL && i <= 10);
    printf("======================\n");
}

/*
@brief Kullan�c�n�n dinleme dizisini ekrana yazd�r�r.
@param username Kullan�c� ad�
@param arr Dinleme ad�mlar�n� i�eren dizi
@param steps Ad�m say�s�
@return
*/
void displayUserListenArray(const char *username, int *arr, int steps) {
    if (!arr || steps <= 0) return;
    printf("=== %s��n Dinleme Dizisi (%d ad�m) ===\n", username, steps);
    int i;
    for (i = 0; i < steps; i++) {
        printf("%3d: %d\n", i+1, arr[i]);
    }
    printf("===================================\n");
}
/*
 @brief input.txt�ten �ark�lar� ve kullan�c�-�ark� atamalar�n� okur.
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
    // 1) �ark�lar� oku
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

    // 2) kullan�c�lar� ve �ark� atamalar�n� oku
    for (uid = 1; uid <= userCount; uid++) {
        char line[64];
        // kullan�c�y� ekle �User#� 
        char uname[16];
        snprintf(uname, sizeof uname, "User%d", uid);
        pushUser(SpotifyApp, uid, uname);

        // blok sonuna kadar sat�r sat�r oku
        while (fgets(line, sizeof line, f)) {
            // bo� sat�rsa bir sonraki kullan�c�ya ge�
            if (line[0] == '\n' || line[0] == '\r') break;
            // sat�r i�indeki ilk tamsay� �ark� id
            char *p = line;
            // ba��nda bo�luk varsa atla
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
    if (userRef->userSongs) {                     // bo� liste kontrol�
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
@brief Uygulaman�n ba�lang�� noktas�, kullan�c� ve �ark� say�s�n� al�r, verileri olu�turur ve sim�lasyonu ba�lat�r.
@return Uygulama ba�ar�yla tamamland���nda 0 d�nd�r�r.
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

