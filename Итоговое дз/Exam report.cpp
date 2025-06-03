#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>

// Структура для хранения информации о студенте
struct Student {
    std::string surname;
    std::string name;
    std::string patronymic;
    unsigned int rating; // 100-балльная шкала
    unsigned int mark;   // 5-балльная шкала
    bool allowed;        // 1 - допущен, 0 - не допущен

    // Перевод рейтинга в оценку (если нужно пересчитать)
    void computeMark5() {
        if (rating >= 90) mark = 5;
        else if (rating >= 75) mark = 4;
        else if (rating >= 60) mark = 3;
        else mark = 2;
    }
};

// Класс для работы с базой данных студентов
class StudentDB {
private:
    std::vector<Student> students;

public:
    // Загрузка студентов из файла (6 значений в строке)
    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) {
            std::cerr << "Ошибка открытия файла.\n";
            return;
        }
        students.clear();
        Student s;
        while (in >> s.surname >> s.name >> s.patronymic >> s.rating >> s.mark >> s.allowed) {
            students.push_back(s);
        }
        in.close();
    }

    // Сохранение студентов в файл (6 значений в строке)
    void saveToFile(const std::string& filename) {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Ошибка открытия файла для записи.\n";
            return;
        }
        for (const auto& s : students) {
            out << s.surname << ' ' << s.name << ' ' << s.patronymic << ' '
                << s.rating << ' ' << s.mark << ' ' << s.allowed << '\n';
        }
    }

    // Красивый табличный вывод всех студентов
    void view() const {
        // Ширины столбцов
        const int wSurname = 12;
        const int wName = 12;
        const int wPatronymic = 15;
        const int wRating = 8;
        const int wMark = 8;
        const int wAllowed = 10;
        const int totalWidth = wSurname + wName + wPatronymic + wRating + wMark + wAllowed + 7;

        // Верхняя линия
        std::cout << std::setfill('=') << std::setw(totalWidth) << "=" << std::setfill(' ') << std::endl;

        // Заголовок
        std::cout << "|"
                  << std::setw(wSurname) << "Фамилия" << "|"
                  << std::setw(wName) << "Имя" << "|"
                  << std::setw(wPatronymic) << "Отчество" << "|"
                  << std::setw(wRating) << "Рейт" << "|"
                  << std::setw(wMark) << "Оценка" << "|"
                  << std::setw(wAllowed) << "Допущен" << "|" << std::endl;

        // Разделительная линия
        std::cout << std::setfill('-') << std::setw(totalWidth) << "-" << std::setfill(' ') << std::endl;

        // Данные студентов
        for (const auto& s : students) {
            std::cout << "|"
                      << std::setw(wSurname) << s.surname << "|"
                      << std::setw(wName) << s.name << "|"
                      << std::setw(wPatronymic) << s.patronymic << "|"
                      << std::setw(wRating) << s.rating << "|"
                      << std::setw(wMark) << s.mark << "|"
                      << std::setw(wAllowed) << (s.allowed ? "Да" : "Нет") << "|" << std::endl;
        }

        // Нижняя линия
        std::cout << std::setfill('=') << std::setw(totalWidth) << "=" << std::setfill(' ') << std::endl;
    }

    // Добавление студента
    void add(const Student& s) {
        students.push_back(s);
    }

    // Удаление студента по индексу
    void remove(int index) {
        if (index >= 0 && index < students.size())
            students.erase(students.begin() + index);
    }

    // Сортировка по фамилии
    void sortBySurname() {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.surname < b.surname;
        });
    }

    // Сортировка по рейтингу и фамилии
    void sortByRatingAndSurname() {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            if (a.rating != b.rating) return a.rating > b.rating;
            else return a.surname < b.surname;
        });
    }

    // Поиск по фамилии с сообщением, если не найдено
    void searchBySurname(const std::string& key) const {
        bool found = false;
        for (const auto& s : students) {
            if (s.surname == key) {
                // Табличный вывод одного студента
                const int wSurname = 12, wName = 12, wPatronymic = 15, wRating = 8, wMark = 8, wAllowed = 10;
                const int totalWidth = wSurname + wName + wPatronymic + wRating + wMark + wAllowed + 7;
                std::cout << std::setfill('=') << std::setw(totalWidth) << "=" << std::setfill(' ') << std::endl;
                std::cout << "|"
                          << std::setw(wSurname) << "Фамилия" << "|"
                          << std::setw(wName) << "Имя" << "|"
                          << std::setw(wPatronymic) << "Отчество" << "|"
                          << std::setw(wRating) << "Рейт" << "|"
                          << std::setw(wMark) << "Оценка" << "|"
                          << std::setw(wAllowed) << "Допущен" << "|" << std::endl;
                std::cout << std::setfill('-') << std::setw(totalWidth) << "-" << std::setfill(' ') << std::endl;
                std::cout << "|"
                          << std::setw(wSurname) << s.surname << "|"
                          << std::setw(wName) << s.name << "|"
                          << std::setw(wPatronymic) << s.patronymic << "|"
                          << std::setw(wRating) << s.rating << "|"
                          << std::setw(wMark) << s.mark << "|"
                          << std::setw(wAllowed) << (s.allowed ? "Да" : "Нет") << "|" << std::endl;
                std::cout << std::setfill('=') << std::setw(totalWidth) << "=" << std::setfill(' ') << std::endl;
                found = true;
            }
        }
        if (!found) {
            std::cout << "Студент с фамилией \"" << key << "\" не найден.\n";
        }
    }

    // Фильтрация по диапазону рейтинга
    void filterByRatingRange(int minR, int maxR) const {
        bool found = false;
        // Табличный вывод заголовка
        const int wSurname = 12, wName = 12, wPatronymic = 15, wRating = 8, wMark = 8, wAllowed = 10;
        const int totalWidth = wSurname + wName + wPatronymic + wRating + wMark + wAllowed + 7;
        std::cout << std::setfill('=') << std::setw(totalWidth) << "=" << std::setfill(' ') << std::endl;
        std::cout << "|"
                  << std::setw(wSurname) << "Фамилия" << "|"
                  << std::setw(wName) << "Имя" << "|"
                  << std::setw(wPatronymic) << "Отчество" << "|"
                  << std::setw(wRating) << "Рейт" << "|"
                  << std::setw(wMark) << "Оценка" << "|"
                  << std::setw(wAllowed) << "Допущен" << "|" << std::endl;
        std::cout << std::setfill('-') << std::setw(totalWidth) << "-" << std::setfill(' ') << std::endl;
        for (const auto& s : students) {
            if (s.rating >= minR && s.rating <= maxR) {
                std::cout << "|"
                          << std::setw(wSurname) << s.surname << "|"
                          << std::setw(wName) << s.name << "|"
                          << std::setw(wPatronymic) << s.patronymic << "|"
                          << std::setw(wRating) << s.rating << "|"
                          << std::setw(wMark) << s.mark << "|"
                          << std::setw(wAllowed) << (s.allowed ? "Да" : "Нет") << "|" << std::endl;
                found = true;
            }
        }
        std::cout << std::setfill('=') << std::setw(totalWidth) << "=" << std::setfill(' ') << std::endl;
        if (!found) {
            std::cout << "Нет студентов с рейтингом в диапазоне [" << minR << ", " << maxR << "].\n";
        }
    }

    // Проверка корректности данных
    void validateData() const {
        std::cout << "Ошибки данных:\n";
        for (const auto& s : students) {
            if (s.rating > 100)
                std::cout << s.surname << ": рейтинг превышает 100.\n";
            if (s.mark < 2 || s.mark > 5)
                std::cout << s.surname << ": некорректная оценка по 5-балльной шкале.\n";
            if (!s.allowed && s.rating > 0)
                std::cout << s.surname << ": не допущен, но имеет рейтинг.\n";
        }
    }

    // Средний рейтинг
    float computeAverageRating() const {
        if (students.empty()) return 0.0f;
        unsigned int total = 0;
        for (const auto& s : students)
            total += s.rating;
        return static_cast<float>(total) / students.size();
    }

    // Выборка студентов с рейтингом выше среднего
    StudentDB extractAboveAverage() const {
        float avg = computeAverageRating();
        StudentDB newDB;
        for (const auto& s : students) {
            if (s.rating > avg)
                newDB.add(s);
        }
        return newDB;
    }

    // Список недопущенных студентов
    void listNotAllowed() const {
        bool found = false;
        // Табличный вывод заголовка
        const int wSurname = 12, wName = 12, wPatronymic = 15, wRating = 8, wMark = 8, wAllowed = 10;
        const int totalWidth = wSurname + wName + wPatronymic + wRating + wMark + wAllowed + 7;
        std::cout << std::setfill('=') << std::setw(totalWidth) << "=" << std::setfill(' ') << std::endl;
        std::cout << "|"
                  << std::setw(wSurname) << "Фамилия" << "|"
                  << std::setw(wName) << "Имя" << "|"
                  << std::setw(wPatronymic) << "Отчество" << "|"
                  << std::setw(wRating) << "Рейт" << "|"
                  << std::setw(wMark) << "Оценка" << "|"
                  << std::setw(wAllowed) << "Допущен" << "|" << std::endl;
        std::cout << std::setfill('-') << std::setw(totalWidth) << "-" << std::setfill(' ') << std::endl;
        for (const auto& s : students) {
            if (!s.allowed) {
                std::cout << "|"
                          << std::setw(wSurname) << s.surname << "|"
                          << std::setw(wName) << s.name << "|"
                          << std::setw(wPatronymic) << s.patronymic << "|"
                          << std::setw(wRating) << s.rating << "|"
                          << std::setw(wMark) << s.mark << "|"
                          << std::setw(wAllowed) << "Нет" << "|" << std::endl;
                found = true;
            }
        }
        std::cout << std::setfill('=') << std::setw(totalWidth) << "=" << std::setfill(' ') << std::endl;
        if (!found) {
            std::cout << "Все студенты допущены.\n";
        }
    }

    // Размер базы данных (для проверки индекса)
    int size() const {
        return students.size();
    }
}; // <-- Закрывающая скобка класса StudentDB

// Меню для управления бд
void menu() {
    StudentDB db;
    while (true) {
        std::cout << "\n1. Загрузить БД\n2. Сохранить БД\n3. Просмотр\n4. Добавить\n5. Удалить\n6. Сортировка по фамилии\n7. Сортировка по рейтингу и фамилии\n8. Поиск по фамилии\n9. Фильтр по рейтингу\n10. Проверка данных\n11. Средний балл и выборка выше среднего\n12. Недопущенные\n0. Выход\n> ";
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите номер пункта меню!\n";
            continue;
        }
        if (choice == 0) break;
        else if (choice == 1) db.loadFromFile("input.txt");
        else if (choice == 2) db.saveToFile("output.txt");
        else if (choice == 3) db.view();
        else if (choice == 4) {
            Student s;
            std::cout << "Введите фамилию, имя, отчество, рейтинг (0-100), оценку (2-5), допущен (1/0): ";
            std::cin >> s.surname >> s.name >> s.patronymic >> s.rating >> s.mark >> s.allowed;
            if (std::cin.fail() || s.rating > 100 || s.mark < 2 || s.mark > 5 || (s.allowed != 0 && s.allowed != 1)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка ввода! Попробуйте снова.\n";
            } else {
                db.add(s);
            }
        }
        else if (choice == 5) {
            int idx;
            std::cout << "Введите номер записи для удаления: ";
            std::cin >> idx;
            if (std::cin.fail() || idx < 0 || idx >= db.size()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: некорректный индекс!\n";
            } else {
                db.remove(idx);
            }
        }
        else if (choice == 6) db.sortBySurname();
        else if (choice == 7) db.sortByRatingAndSurname();
        else if (choice == 8) {
            std::string key;
            std::cout << "Введите фамилию для поиска: ";
            std::cin >> key;
            db.searchBySurname(key);
        }
        else if (choice == 9) {
            int min, max;
            std::cout << "Введите диапазон рейтинга (min max): ";
            std::cin >> min >> max;
            if (std::cin.fail() || min < 0 || max > 100 || min > max) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: некорректный диапазон!\n";
            } else {
                db.filterByRatingRange(min, max);
            }
        }
        else if (choice == 10) db.validateData();
        else if (choice == 11) {
            StudentDB newDB = db.extractAboveAverage();
            newDB.saveToFile("above_avg.txt");
            std::cout << "Выборка студентов выше среднего сохранена в above_avg.txt\n";
        }
        else if (choice == 12) db.listNotAllowed();
        else {
            std::cout << "Нет такого пункта меню.\n";
        }
    }
}

int main() {
    menu();
    return 0;
}
