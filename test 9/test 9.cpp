#include <iostream>
#include <fstream>
#include <clocale>

using namespace std;

enum Genre {
    COMEDY,
    THRILLER,
    DRAMA,
    ACTION,
    FANTASY
};

struct Duration {
    int hours;
    int minutes;
};

struct Film {
    char title[100];
    char director[100];
    Genre genre;
    int year;
    Duration duration;
    float budget;
    float rating;
    char mainActor[100];
};

int cmp(const char a[], const char b[]) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i++;
    }
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
}

void printFilm(const Film& f) {
    const char* genreNames[] = { "Комедия", "Триллер", "Драма", "Боевик", "Фэнтези" };
    cout << "Название: " << f.title << "\n";
    cout << "Режиссёр: " << f.director << "\n";
    cout << "Жанр: " << genreNames[f.genre] << "\n";
    cout << "Год: " << f.year << "\n";
    cout << "Длительность: " << f.duration.hours << "ч " << f.duration.minutes << "мин\n";
    cout << "Бюджет: " << f.budget << " млн\n";
    cout << "Рейтинг: " << f.rating << "\n";
    cout << "Главный актёр: " << f.mainActor << "\n";
    cout << "------------------------\n";
}

int filterByActor(const Film src[], int srcSize, Film dst[], const char actor[]) {
    int count = 0;
    for (int i = 0; i < srcSize; ++i) {
        if (cmp(src[i].mainActor, actor) == 0) {
            dst[count++] = src[i];
        }
    }
    return count;
}

void sortByTitle(Film arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (cmp(arr[j].title, arr[j + 1].title) > 0) {
                Film temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void findFilmByTitle(const Film arr[], int n, const char title[]) {
    for (int i = 0; i < n; ++i) {
        if (cmp(arr[i].title, title) == 0) {
            printFilm(arr[i]);
            return;
        }
    }
    cout << "Фильм \"" << title << "\" не найден.\n";
}

void changeFilm(Film& f) {
    cout << "Введите новое название: ";
    cin >> f.title;
    cout << "Введите нового режиссёра: ";
    cin >> f.director;
    int g;
    cout << "Жанр (0-Комедия,1-Триллер,2-Драма,3-Боевик,4-Фэнтези): ";
    cin >> g;
    f.genre = static_cast<Genre>(g);
    cout << "Год: ";
    cin >> f.year;
    cout << "Длительность (часы и минуты через пробел): ";
    cin >> f.duration.hours >> f.duration.minutes;
    cout << "Бюджет (млн): ";
    cin >> f.budget;
    cout << "Рейтинг: ";
    cin >> f.rating;
    cout << "Главный актёр: ";
    cin >> f.mainActor;
}

bool isDirectorDavid(const char director[]) {
    const char david[] = "David";
    int i = 0;
    while (david[i] != 0 && director[i] != 0) {
        if (director[i] != david[i]) return false;
        i++;
    }
    return (director[i] == ' ' || director[i] == 0);
}

int filterDavidThrillers(const Film src[], int srcSize, Film dst[]) {
    int count = 0;
    for (int i = 0; i < srcSize; ++i) {
        if (src[i].genre == THRILLER && isDirectorDavid(src[i].director)) {
            dst[count++] = src[i];
        }
    }
    return count;
}

void wrapperDavidThrillers(const Film arr[], int n) {
    Film temp[20];
    int cnt = filterDavidThrillers(arr, n, temp);
    if (cnt == 0) {
        cout << "Нет триллеров с режиссёром Дэвид.\n";
        return;
    }
    cout << "=== Триллеры режиссёров по имени Дэвид ===\n";
    for (int i = 0; i < cnt; ++i) {
        printFilm(temp[i]);
    }
}

void top4Expensive2020s(const Film arr[], int n) {
    Film temp[20];
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i].year >= 2020 && arr[i].year <= 2029) {
            temp[cnt++] = arr[i];
        }
    }
    if (cnt == 0) {
        cout << "Нет фильмов 2020-х годов.\n";
        return;
    }
    for (int i = 0; i < cnt - 1; ++i) {
        for (int j = 0; j < cnt - i - 1; ++j) {
            if (temp[j].budget < temp[j + 1].budget) {
                Film t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    int top = (cnt < 4) ? cnt : 4;
    cout << "=== " << top << " самых дорогих фильмов 2020-х ===\n";
    for (int i = 0; i < top; ++i) {
        cout << temp[i].title << " (" << temp[i].budget << " млн)\n";
    }
}

void updateRatingsFromFile(Film arr[], int n, const char* filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Не удалось открыть файл " << filename << " для чтения.\n";
        return;
    }

    char name[100];
    float newRating;
    int updated = 0;

    while (fin >> name >> newRating) {
        bool found = false;
        for (int i = 0; i < n; ++i) {
            if (cmp(arr[i].title, name) == 0) {
                arr[i].rating = newRating;
                found = true;
                updated++;
                break;
            }
        }
        if (!found) {
            cout << "Фильм \"" << name << "\" не найден, рейтинг не обновлён.\n";
        }
    }
    fin.close();
    cout << "Обновлено рейтингов: " << updated << "\n";
}

void saveToBinary(const Film arr[], int size, const char* filename) {
    ofstream fout(filename, ios::binary | ios::out);
    if (!fout.is_open()) {
        cout << "Ошибка открытия файла " << filename << " для записи.\n";
        return;
    }
    fout.write(reinterpret_cast<const char*>(arr), size * sizeof(Film));
    fout.close();
    cout << "Массив структур сохранён в бинарный файл " << filename << "\n";
}

int loadFromBinary(Film arr[], int maxSize, const char* filename) {
    ifstream fin(filename, ios::binary | ios::in);
    if (!fin.is_open()) {
        cout << "Ошибка открытия файла " << filename << " для чтения.\n";
        return 0;
    }
    fin.seekg(0, ios::end);
    streampos fileSize = fin.tellg();
    fin.seekg(0, ios::beg);

    int elements = fileSize / sizeof(Film);
    if (elements > maxSize) {
        cout << "Файл содержит больше элементов (" << elements
             << "), чем может вместить массив (" << maxSize << ").\n";
        elements = maxSize;
    }
    fin.read(reinterpret_cast<char*>(arr), elements * sizeof(Film));
    fin.close();
    cout << "Из бинарного файла " << filename << " загружено " << elements << " записей.\n";
    return elements;
}

int main() {
    setlocale(LC_ALL, "Russian");

    Film films[20] = {
        {"Drive", "Nicolas Winding Refn", THRILLER, 2011, {1,40}, 15.0, 7.8, "Ryan Gosling"},
        {"La La Land", "Damien Chazelle", DRAMA, 2016, {2,8}, 30.0, 8.1, "Ryan Gosling"},
        {"The Notebook", "Nick Cassavetes", DRAMA, 2004, {2,3}, 29.0, 7.8, "Ryan Gosling"},
        {"Crazy Stupid Love", "Glenn Ficarra", COMEDY, 2011, {1,58}, 50.0, 7.4, "Ryan Gosling"},
        {"Barbie", "Greta Gerwig", COMEDY, 2023, {1,54}, 145.0, 7.1, "Ryan Gosling"},
        {"Blade Runner 2049", "Denis Villeneuve", FANTASY, 2017, {2,44}, 185.0, 8.0, "Ryan Gosling"},
        {"The Place Beyond the Pines", "Derek Cianfrance", DRAMA, 2012, {2,20}, 15.0, 7.3, "Ryan Gosling"},
        {"Gangster Squad", "Ruben Fleischer", ACTION, 2013, {1,53}, 60.0, 6.7, "Ryan Gosling"},
        {"Only God Forgives", "Nicolas Winding Refn", THRILLER, 2013, {1,30}, 12.0, 5.7, "Ryan Gosling"},
        {"The Big Short", "Adam McKay", DRAMA, 2015, {2,10}, 28.0, 7.8, "Ryan Gosling"},
        {"First Man", "Damien Chazelle", DRAMA, 2018, {2,21}, 59.0, 7.5, "Ryan Gosling"},
        {"Se7en", "David Fincher", THRILLER, 1995, {2,7}, 33.0, 8.6, "Brad Pitt"},
        {"Fight Club", "David Fincher", DRAMA, 1999, {2,19}, 63.0, 8.8, "Brad Pitt"},
        {"The Social Network", "David Fincher", DRAMA, 2010, {2,0}, 40.0, 8.1, "Jesse Eisenberg"},
        {"Zodiac", "David Fincher", THRILLER, 2007, {2,37}, 65.0, 7.7, "Jake Gyllenhaal"},
        {"Gone Girl", "David Fincher", THRILLER, 2014, {2,29}, 61.0, 8.1, "Ben Affleck"},
        {"Mank", "David Fincher", DRAMA, 2020, {2,11}, 25.0, 7.0, "Gary Oldman"},
        {"The Killer", "David Fincher", THRILLER, 2023, {1,58}, 30.0, 6.8, "Michael Fassbender"},
        {"Inception", "Christopher Nolan", ACTION, 2010, {2,28}, 160.0, 8.7, "Leonardo DiCaprio"},
        {"Interstellar", "Christopher Nolan", FANTASY, 2014, {2,49}, 165.0, 8.5, "Matthew McConaughey"}
    };

    Film goslingFilms[20];
    int goslingCount = filterByActor(films, 20, goslingFilms, "Ryan Gosling");
    cout << "Фильмы с Райаном Гослингом (всего " << goslingCount << "):\n";
    for (int i = 0; i < goslingCount; ++i) printFilm(goslingFilms[i]);

    sortByTitle(goslingFilms, goslingCount);
    cout << "\n=== Отсортированные по названию ===\n";
    for (int i = 0; i < goslingCount; ++i) printFilm(goslingFilms[i]);

    cout << "\n=== Поиск фильма 'Drive' ===\n";
    findFilmByTitle(films, 20, "Drive");

    cout << "\n=== Изменение данных первого фильма ===\n";
    changeFilm(films[0]);
    cout << "\nИзменённый фильм:\n";
    printFilm(films[0]);

    wrapperDavidThrillers(films, 20);
    top4Expensive2020s(films, 20);

    cout << "\n========== Задание 1: обновление рейтингов из текстового файла ==========\n";
    updateRatingsFromFile(films, 20, "ratings.txt");
    cout << "\nТекущие рейтинги после обновления (несколько примеров):\n";
    cout << "Drive rating = " << films[0].rating << "\n";
    cout << "La La Land rating = " << films[1].rating << "\n";
    cout << "Se7en rating = " << films[11].rating << "\n";
    cout << "Inception rating = " << films[18].rating << "\n";

    cout << "\n========== Задание 2: работа с бинарным файлом ==========\n";
    const char* binaryFile = "films.bin";

    saveToBinary(films, 20, binaryFile);

    Film loadedFilms[20];
    int loadedCount = loadFromBinary(loadedFilms, 20, binaryFile);

    cout << "\nПроверка загруженных данных (первые 3 фильма):\n";
    for (int i = 0; i < 3 && i < loadedCount; ++i) {
        printFilm(loadedFilms[i]);
    }

    return 0;
}
