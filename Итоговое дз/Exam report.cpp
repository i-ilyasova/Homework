#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>

// Хранение информации о студенте.
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

    // Вывод данных о студенте в табличном виде
    void print() const {
        std::cout << std::setw(12) << surname
                  << std::setw(12) << name
                  << std::setw(15) << patronymic
                  << std::setw(8) << rating
                  << std::setw(8) << mark
                  << std::setw(10) << (allowed ? "Да" : "Нет") << '\n';
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
            // Если хотите пересчитывать оценку по рейтингу, раскомментируйте следующую строку:
            // s.computeMark5();
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

    // Вывод всех студентов
    void view() const {
        std::cout << std::setw(12) << "Фамилия"
                  << std::setw(12) << "Имя"
                  << std::setw(15) << "Отчество"
                  << std::setw(8) << "Рейтинг"
                  << std::setw(8) << "Оценка"
                  << std::setw(10) << "Допущен" << '\n';
        for (const auto& s : students)
            s.print();
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
                s.print();
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
        for (const auto& s : students) {
            if (s.rating >= minR && s.rating <= maxR) {
                s.print();
                found = true;
            }
        }
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
        std::cout << "Недопущенные студенты:\n";
        for (const auto& s : students) {
            if (!s.allowed) {
                s.print();
                found = true;
            }
        }
        if (!found) {
            std::cout << "Все студенты допущены.\n";
        }
    }

    // Размер базы данных (для проверки индекса)
    int size() const {
        return students.size();
    }
};

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
