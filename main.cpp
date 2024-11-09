#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

struct Person {
    std::string name;
    std::string surname;
    std::string phone;

    // Метод для вывода информации о человеке
    void print() const {
        std::cout << name << " " << surname << ": " << phone << std::endl;
    }
};

template<typename T>
auto createComparator(T Person::*member1, T Person::*member2) {
    return [member1, member2](const Person &a, const Person &b) {
        if (a.*member1 != b.*member1) {
            return a.*member1 < b.*member1; // Сравниваем по первому члену
        }
        return a.*member2 < b.*member2; // Если равны, сравниваем по второму
    };
}

std::vector<Person> readPersonsFromFile(const std::string& filename) {
    std::vector<Person> persons;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return persons;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Person person;

        // Чтение фамилии и имени
        std::getline(iss, person.surname, ' ');
        std::getline(iss, person.name, ':');

        // Удаление пробелов
        person.name.erase(0, person.name.find_first_not_of(" "));

        // Чтение телефона
        std::getline(iss, person.phone);
        // Удаление пробелов
        person.phone.erase(0, person.phone.find_first_not_of(" ")); 

        persons.push_back(person);
    }

    file.close();
    return persons;
}

int main() {
    const std::string filename = "data.txt";
    std::vector<Person> people = readPersonsFromFile(filename);

    int choice;
    std::cout << "Для сортировки по именам введите – 1, по фамилиям введите- 2, по телефонам введите- 3: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            std::stable_sort(people.begin(), people.end(), createComparator(&Person::name, &Person::surname));
            std::cout << "Сортировка по именам:\n";
            for (const auto &person : people) {
                std::cout << person.name << " " << person.surname << ": " << person.phone << std::endl;
            }
            break;

        case 2:
            std::stable_sort(people.begin(), people.end(), createComparator(&Person::surname, &Person::name));
            std::cout << "Сортировка по фамилиям:\n";
            for (const auto &person : people) {
                std::cout << person.surname << " " << person.name << ": " << person.phone << std::endl;
            }
            break;

        case 3:
            std::stable_sort(people.begin(), people.end(), createComparator(&Person::phone, &Person::surname));
            std::cout << "Сортировка по телефонам:\n";
            for (const auto &person : people) {
                std::cout << person.phone << ": " << person.surname << " " << person.name << std::endl;
            }
            break;

        default:
            std::cout << "Некорректный ввод." << std::endl;
            return 1;
    }

    return 0;
}