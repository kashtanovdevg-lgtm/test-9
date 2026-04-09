#include <iostream>
#include <clocale>

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

void prt(const Film& f) {
    const char* genreNames[] = { "Комедия", "Триллер", "Драма", "Боевик", "Фэнтези" };
    std::cout << "Название: " << f.title << "\n";
    std::cout << "Режиссёр: " << f.director << "\n";
    std::cout << "Жанр: " << genreNames[f.genre] << "\n";
    std::cout << "Год: " << f.year << "\n";
    std::cout << "Длительность: " << f.duration.hours << "ч " << f.duration.minutes << "мин\n";
    std::cout << "Бюджет: " << f.budget << " млн\n";
    std::cout << "Рейтинг: " << f.rating << "\n";
    std::cout << "Главный актёр: " << f.mainActor << "\n";
    std::cout << "------------------------\n";
}

int fAc(const Film src[], int srcSize, Film dst[], const char actor[]) {
    int count = 0;
    for (int i = 0; i < srcSize; ++i) {
        if (cmp(src[i].mainActor, actor) == 0) {
            dst[count++] = src[i];
        }
    }
    return count;
}

void srt(Film arr[], int n) {
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

void fnd(const Film arr[], int n, const char title[]) {
    for (int i = 0; i < n; ++i) {
        if (cmp(arr[i].title, title) == 0) {
            prt(arr[i]);
            return;
        }
    }
    std::cout << "Фильм \"" << title << "\" не найден.\n";
}

void chg(Film& f) {
    std::cout << "Введите новое название: ";
    std::cin >> f.title;
    std::cout << "Введите нового режиссёра: ";
    std::cin >> f.director;
    int g;
    std::cout << "Жанр (0-Комедия,1-Триллер,2-Драма,3-Боевик,4-Фэнтези): ";
    std::cin >> g;
    f.genre = static_cast<Genre>(g);
    std::cout << "Год: ";
    std::cin >> f.year;
    std::cout << "Длительность (часы и минуты через пробел): ";
    std::cin >> f.duration.hours >> f.duration.minutes;
    std::cout << "Бюджет (млн): ";
    std::cin >> f.budget;
    std::cout << "Рейтинг: ";
    std::cin >> f.rating;
    std::cout << "Главный актёр: ";
    std::cin >> f.mainActor;
}

bool isD(const char director[]) {
    const char david[] = "David";
    int i = 0;
    while (david[i] != 0 && director[i] != 0) {
        if (director[i] != david[i]) return false;
        i++;
    }
    return (director[i] == ' ' || director[i] == 0);
}

int fDv(const Film src[], int srcSize, Film dst[]) {
    int count = 0;
    for (int i = 0; i < srcSize; ++i) {
        if (src[i].genre == THRILLER && isD(src[i].director)) {
            dst[count++] = src[i];
        }
    }
    return count;
}

void wrp(const Film arr[], int n) {
    Film temp[20];
    int cnt = fDv(arr, n, temp);
    if (cnt == 0) {
        std::cout << "Нет триллеров с режиссёром Дэвид.\n";
        return;
    }
    std::cout << "=== Триллеры режиссёров по имени Дэвид ===\n";
    for (int i = 0; i < cnt; ++i) {
        prt(temp[i]);
    }
}

void top4(const Film arr[], int n) {
    Film temp[20];
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i].year >= 2020 && arr[i].year <= 2029) {
            temp[cnt++] = arr[i];
        }
    }
    if (cnt == 0) {
        std::cout << "Нет фильмов 2020-х годов.\n";
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
    int top;
    if (cnt < 4) {
        top = cnt;
    }
    else {
        top = 4;
    }
    std::cout << "=== " << top << " самых дорогих фильмов 2020-х ===\n";
    for (int i = 0; i < top; ++i) {
        std::cout << temp[i].title << " (" << temp[i].budget << " млн)\n";
    }
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
    int goslingCount = fAc(films, 20, goslingFilms, "Ryan Gosling");
    std::cout << "Фильмы с Райаном Гослингом (всего " << goslingCount << "):\n";
    for (int i = 0; i < goslingCount; ++i) prt(goslingFilms[i]);

    srt(goslingFilms, goslingCount);
    std::cout << "\n=== Отсортированные по названию ===\n";
    for (int i = 0; i < goslingCount; ++i) prt(goslingFilms[i]);

    std::cout << "\n=== Поиск фильма 'Drive' ===\n";
    fnd(films, 20, "Drive");

    std::cout << "\n=== Изменение данных первого фильма ===\n";
    chg(films[0]);
    std::cout << "\nИзменённый фильм:\n";
    prt(films[0]);

    wrp(films, 20);
    top4(films, 20);

}