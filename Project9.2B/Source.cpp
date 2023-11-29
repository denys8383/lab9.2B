#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define SORT

const int MAX_NAME_LENGTH = 20;

// Об'єднання для курсів
union Course {
    int value;
};

// Об'єднання для спеціальностей
union Specialty {
    int value;
};

const int NUM_COURSES = 5;
const int NUM_SPECIALTIES = 8;

// Функція для конвертації значення спеціальності в назву
const char* specialtiesToString(Specialty specialty) {
    switch (specialty.value) {
    case 0:
        return "Mathematics";
    case 1:
        return "Physics";
    case 2:
        return "Crafting";
    case 3:
        return "Computer Science";
    case 4:
        return "Mathematics and Economics";
    case 5:
        return "Physics and Computer Science";
    case 6:
        return "Labour Training";
    case 7:
        return "Informatics";
    default:
        return "Unknown";
    }
}

// Структура, що представляє інформацію про студента
struct Student {
    int studentNumber;
    char lastName[MAX_NAME_LENGTH];
    Course course;
    Specialty specialty;
    double physics;
    double math;
    union {
        double it;  // для "Комп'ютерних наук"
        double pedagogy;  // для інших спеціальностей
        double numericalMethods;  // для "Інформатики"
    };
};

// Функція порівняння для qsort на основі курсу
int compareStudentsByCourse(const void* a, const void* b) {
    const Student* studentA = static_cast<const Student*>(a);
    const Student* studentB = static_cast<const Student*>(b);

    // Порівняння за курсом
    return studentA->course.value - studentB->course.value;
}

// Функція порівняння для qsort на основі спеціальності
int compareStudentsBySpecialty(const void* a, const void* b) {
    const Student* studentA = static_cast<const Student*>(a);
    const Student* studentB = static_cast<const Student*>(b);

    // Порівняння за спеціальністю
    return studentA->specialty.value - studentB->specialty.value;
}

// Функція порівняння для qsort на основі прізвища в порядку спадання
int compareStudentsByLastName(const void* a, const void* b) {
    const Student* studentA = static_cast<const Student*>(a);
    const Student* studentB = static_cast<const Student*>(b);

    // Порівняти за прізвищем у порядку спадання
    return -strcmp(studentA->lastName, studentB->lastName);
}

// Функція для введення даних про студентів
void inputStudents(Student* students, int numberOfStudents) {
    const char* lastNames[] = { "Vovk", "Mazurok", "Ushakov", "Kojushko", "Zarubich", "Simonenko", "Barchuk", "Osadchyi", "Kalyna" };
    const Specialty specialties[] = { {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7} };

    for (int i = 0; i < numberOfStudents; ++i) {
        strncpy(students[i].lastName, lastNames[i % (sizeof(lastNames) / sizeof(lastNames[0]))], MAX_NAME_LENGTH - 1);
        students[i].lastName[MAX_NAME_LENGTH - 1] = '\0';

        students[i].specialty = specialties[i % (sizeof(specialties) / sizeof(specialties[0]))];

        students[i].course.value = rand() % NUM_COURSES + 1;
        students[i].physics = rand() % 3 + 3;
        students[i].math = rand() % 3 + 3;

        if (students[i].specialty.value == 3) {
            students[i].it = rand() % 3 + 3;
        }
        else if (students[i].specialty.value == 7) {
            students[i].numericalMethods = rand() % 3 + 3;
        }
        else {
            students[i].pedagogy = rand() % 3 + 3;
        }

        students[i].studentNumber = i + 1;
    }
}

// Функція для виведення таблиці студентів та розрахунку кількості та відсотка студентів
void displayTable(const Student* students, int numberOfStudents) {
    int countWithoutThrees = 0;
    int countBelowFour = 0;

    std::cout << "----------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "| No |  Surname   |Course|            Speciality        |  Physics |    Math   |  Programming  |   Pedagogy   | Numerical methods|" << std::endl;
    std::cout << "----------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    for (int i = 0; i < numberOfStudents; ++i) {
        std::cout << "| " << std::setw(2) << students[i].studentNumber << " | " << std::setw(10) << students[i].lastName << " | "
            << std::setw(4) << students[i].course.value << " | " << std::setw(28) << specialtiesToString(students[i].specialty) << " | "
            << std::setw(8) << students[i].physics << " | " << std::setw(9) << students[i].math << " | ";

        if (students[i].specialty.value == 3) {
            std::cout << std::setw(13) << students[i].it << " |              |              |";
        }
        else if (students[i].specialty.value == 4 || students[i].specialty.value == 5 || students[i].specialty.value == 6) {
            std::cout << "              |" << std::setw(13) << students[i].pedagogy << " |              |";
        }
        else if (students[i].specialty.value == 7) {
            std::cout << "              |              |" << std::setw(13) << students[i].numericalMethods << " |";
        }
        else {
            std::cout << "              |              |              |";
        }

        std::cout << std::endl;

        if (students[i].physics >= 4 && students[i].math >= 4 && ((students[i].specialty.value == 3 && students[i].it >= 4) || (students[i].specialty.value == 7 && students[i].numericalMethods >= 4) || (students[i].specialty.value != 3 && students[i].specialty.value != 7 && students[i].pedagogy >= 4))) {
            countWithoutThrees++;
        }

        double averageGrade = (students[i].physics + students[i].math + ((students[i].specialty.value == 3) ? students[i].it : ((students[i].specialty.value == 7) ? students[i].numericalMethods : students[i].pedagogy))) / 3.0;

        if (averageGrade < 4.0) {
            countBelowFour++;
        }
    }

    std::cout << "----------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    std::cout << "1. Number of students without threes: " << countWithoutThrees << std::endl;
    std::cout << "2. Percentage of students with an average grade below 4: " << std::fixed << std::setprecision(2)
        << (static_cast<double>(countBelowFour) / numberOfStudents) * 100 << "%" << std::endl;
}

// Функція бінарного пошуку для пошуку студента із заданим прізвищем, курсом та спеціальністю
int binarySearch(const Student* students, int numberOfStudents, const char* lastName, int course, Specialty specialty) {
    for (int i = 0; i < numberOfStudents; ++i) {
        const Student& student = students[i];

        if (strcmp(student.lastName, lastName) == 0 && student.course.value == course && student.specialty.value == specialty.value) {
            // Студента знайдено
            return i;
        }
    }

    // Студента не знайдено
    return -1;
}

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));

    int numberOfStudents;
    std::cout << "Enter the number of students: ";
    std::cin >> numberOfStudents;

    Student* students = new Student[numberOfStudents];

    inputStudents(students, numberOfStudents);

#ifdef SEARCH
    // Вивести невідсортовану таблицю та обчислення
    std::cout << "Unsorted Table:" << std::endl;
    displayTable(students, numberOfStudents);

    qsort(students, numberOfStudents, sizeof(Student), compareStudentsByLastName);

    // Критерії пошуку вхідних даних
    char searchLastName[MAX_NAME_LENGTH];
    int searchCourse;
    int searchSpecialty;

    std::cout << "\nEnter last name to search: ";
    std::cin >> searchLastName;
    std::cout << "Enter course to search: ";
    std::cin >> searchCourse;
    std::cout << "Enter specialty to search (0 for MATHEMATICS, 1 for PHYSICS, 2 for CRAFTING, 3 for COMPUTER_SCIENCE, 4 for MATHEMATICS_AND_ECO, 5 for PHYSICS_AND_COMPUTER, 6 for LABOUR_TRAINING, 7 for INFORMATICS): ";
    std::cin >> searchSpecialty;

    // Перетворити searchSpecialty у значення переліку
    Specialty searchEnumSpecialty = { searchSpecialty };

    // Виконати бінарний пошук
    int foundIndex = binarySearch(students, numberOfStudents, searchLastName, searchCourse, searchEnumSpecialty);

    if (foundIndex != -1) {
        std::cout << "Student found" << std::endl;
        std::cout << "Details of the found student:" << std::endl;
        std::cout << "Last name: " << students[foundIndex].lastName << std::endl;
        std::cout << "Course: " << students[foundIndex].course.value << std::endl;
        std::cout << "Specialty: " << specialtiesToString(students[foundIndex].specialty) << std::endl;
    }
    else {
        std::cout << "Student not found." << std::endl;
    }

    return foundIndex;
#endif

#ifdef SORT
    char continueSorting;
    do {
        // Вивести невідсортовану таблицю та обчислення
        std::cout << "Unsorted Table:" << std::endl;
        displayTable(students, numberOfStudents);
        std::cout << "\nDo you want to continue with the next sorting step? (y/n): ";
        std::cin >> continueSorting;
        if (continueSorting != 'y') {
            break;
        }
        // Сортування за курсом
        qsort(students, numberOfStudents, sizeof(Student), compareStudentsByCourse);
        std::cout << "\nSorted by Course:" << std::endl;
        displayTable(students, numberOfStudents);

        std::cout << "\nDo you want to continue with the next sorting step? (y/n): ";
        std::cin >> continueSorting;
        if (continueSorting != 'y') {
            break;
        }

        // Сортування за спеціальністю
        qsort(students, numberOfStudents, sizeof(Student), compareStudentsBySpecialty);
        std::cout << "\nSorted by Specialty:" << std::endl;
        displayTable(students, numberOfStudents);

        std::cout << "\nDo you want to continue with the next sorting step? (y/n): ";
        std::cin >> continueSorting;
        if (continueSorting != 'y') {
            break;
        }

        // Сортування за прізвищем у порядку спадання
        qsort(students, numberOfStudents, sizeof(Student), compareStudentsByLastName);
        std::cout << "\nSorted by Last Name in Descending Order:" << std::endl;
        displayTable(students, numberOfStudents);

        std::cout << "\nDo you want to continue with the next sorting step? (y/n): ";
        std::cin >> continueSorting;
    } while (continueSorting == 'y');

    // Вільна пам'ять
    delete[] students;

    return 0;
#endif
}
