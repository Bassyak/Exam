#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;

struct Task {
    string title;
    int priority;
    string description;
    time_t dueTime;

    Task(string t, int p, string d, time_t due) : title(t), priority(p), description(d), dueTime(due) {}
};


bool compareByPriority(const Task& a, const Task& b) {
    return a.priority > b.priority;
}


bool compareByTime(const Task& a, const Task& b) {
    return difftime(a.dueTime, b.dueTime) < 0;
}


void printTask(const Task& task) {
    tm ltm;
    localtime_s(&ltm, &task.dueTime);
    cout << "Title: " << task.title << "\nPriority: " << task.priority << "\nDescription: " << task.description
        << "\nDue Time: " << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday << " "
        << ltm.tm_hour << ":" << ltm.tm_min << endl;
}

int main() {
    vector<Task> tasks;
    char option;
    string title, description;
    int priority;
    time_t dueTime;
    struct tm timeStruct = {};

    while (true) {
        cout << "1. Add Task\n2. Delete Task\n3. Edit Task\n4. Search Task\n5. Display Tasks\n6. Exit\nEnter option: ";
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (option) {
        case '1': { // Добавление дела
            cout << "Enter task title: ";
            getline(cin, title);

            cout << "Enter priority (0-100): ";
            cin >> priority;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода

            cout << "Enter description: ";
            getline(cin, description);

            cout << "Enter due date and time (YYYY MM DD HH MM): ";
            cin >> timeStruct.tm_year >> timeStruct.tm_mon >> timeStruct.tm_mday
                >> timeStruct.tm_hour >> timeStruct.tm_min;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода

            // tm_year is years since 1900
            timeStruct.tm_year -= 1900;
            // tm_mon is 0 based
            timeStruct.tm_mon -= 1;
            // tm_isdst is Daylight Saving Time flag
            timeStruct.tm_isdst = -1;

            dueTime = mktime(&timeStruct);

            // Создание и добавление задачи в список
            tasks.push_back(Task(title, priority, description, dueTime));

            cout << "Task added successfully." << endl;
            break;
        }
        case '2': { // Удаление дела
            cout << "Enter task title to delete: ";
            getline(cin, title);

            auto it = remove_if(tasks.begin(), tasks.end(), [&title](const Task& task) {
                return task.title == title;
                });

            if (it != tasks.end()) {
                tasks.erase(it, tasks.end());
                cout << "Task deleted successfully." << endl;
            }
            else {
                cout << "Task not found." << endl;
            }
            break;
        }
        case '3': {
            cout << "Enter the title of the task to edit: ";
            getline(cin, title);

            auto it = find_if(tasks.begin(), tasks.end(), [&title](const Task& task) {
                return task.title == title;
                });

            if (it != tasks.end()) {
                cout << "Editing task: " << it->title << endl;

                cout << "Enter new title (leave blank to keep current): ";
                getline(cin, title);
                if (!title.empty()) {
                    it->title = title;
                }

                cout << "Enter new priority (0-100, leave blank to keep current): ";
                string input;
                getline(cin, input);
                if (!input.empty()) {
                    priority = stoi(input);
                    it->priority = priority;
                }

                cout << "Enter new description (leave blank to keep current): ";
                getline(cin, description);
                if (!description.empty()) {
                    it->description = description;
                }

                cout << "Enter new due date and time (YYYY MM DD HH MM, leave blank to keep current): ";

                getline(cin, input);
                if (!input.empty()) {
                    stringstream ss(input);
                    ss >> timeStruct.tm_year >> timeStruct.tm_mon >> timeStruct.tm_mday
                        >> timeStruct.tm_hour >> timeStruct.tm_min;

                    if (ss.fail()) {
                        cout << "Invalid date and time format." << endl;
                    }
                    else {
                        timeStruct.tm_year -= 1900;
                        timeStruct.tm_mon -= 1;
                        timeStruct.tm_isdst = -1;

                        dueTime = mktime(&timeStruct);
                        it->dueTime = dueTime;
                        cout << "Task edited successfully." << endl;
                    }
                }
            }
            else {
                cout << "Task not found." << endl;
            }
            break;
        }
        case '4': { // Поиск дела
            cout << "Search by:\n1. Title\n2. Priority\n3. Description\n4. Due Time\nEnter search option: ";
            char searchOption;
            cin >> searchOption;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода

            switch (searchOption) {
            case '1': { // Поиск по названию
                cout << "Enter title to search for: ";
                getline(cin, title);
                for (const auto& task : tasks) {
                    if (task.title == title) {
                        printTask(task);
                    }
                }
                break;
            }
            case '2': { // Поиск по приоритету
                cout << "Enter priority to search for: ";
                cin >> priority;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода
                for (const auto& task : tasks) {
                    if (task.priority == priority) {
                        printTask(task);
                    }
                }
                break;
            }
            case '3': { // Поиск по описанию
                cout << "Enter description keyword to search for: ";
                getline(cin, description);
                for (const auto& task : tasks) {
                    if (task.description.find(description) != string::npos) {
                        printTask(task);
                    }
                }
                break;
            }
            case '4': { // Поиск по времени исполнения
                cout << "Enter due time to search for (YYYY MM DD HH MM): ";
                cin >> timeStruct.tm_year >> timeStruct.tm_mon >> timeStruct.tm_mday
                    >> timeStruct.tm_hour >> timeStruct.tm_min;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода

                // tm_year is years since 1900
                timeStruct.tm_year -= 1900;
                // tm_mon is 0 based
                timeStruct.tm_mon -= 1;
                // tm_isdst is Daylight Saving Time flag
                timeStruct.tm_isdst = -1;

                dueTime = mktime(&timeStruct);
                for (const auto& task : tasks) {
                    if (task.dueTime == dueTime) {
                        printTask(task);
                    }
                }
                break;
            }
            default:
                cout << "Invalid search option, please try again." << endl;
            }
            break;
        }
        case '5': { // Отображение списка дел
            cout << "Display tasks for:\n1. Today\n2. This Week\n3. This Month\nEnter display option: ";
            char displayOption;
            cin >> displayOption;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            time_t now = time(nullptr);
            tm ltm_now;
            localtime_s(&ltm_now, &now);

            vector<Task> filteredTasks;
            for (const auto& task : tasks) {
                tm ltm_task;
                localtime_s(&ltm_task, &task.dueTime);

                if (displayOption == '1' && ltm_now.tm_year == ltm_task.tm_year && ltm_now.tm_yday == ltm_task.tm_yday) {
                    filteredTasks.push_back(task);
                }
                else if (displayOption == '2' && ltm_now.tm_year == ltm_task.tm_year && ltm_now.tm_yday / 7 == ltm_task.tm_yday / 7) {
                    filteredTasks.push_back(task);
                }
                else if (displayOption == '3' && ltm_now.tm_year == ltm_task.tm_year && ltm_now.tm_mon == ltm_task.tm_mon) {
                    filteredTasks.push_back(task);
                }
            }

            cout << "Sort tasks by:\n1. Priority\n2. Due Time\nEnter sort option: ";
            char sortOption;
            cin >> sortOption;

            if (sortOption == '1') {
                sort(filteredTasks.begin(), filteredTasks.end(), compareByPriority);
            }
            else if (sortOption == '2') {
                sort(filteredTasks.begin(), filteredTasks.end(), compareByTime);
            }
            else {
                cout << "Invalid sort option, defaulting to sort by priority." << endl;
                sort(filteredTasks.begin(), filteredTasks.end(), compareByPriority);
            }

            for (const auto& task : filteredTasks) {
                printTask(task);
            }

            if (filteredTasks.empty()) {
                cout << "No tasks found for this period." << endl;
            }

            break;
        }
        case '6':
            return 0;
        default:
            cout << "Invalid option, please try again." << endl;
        }
    }

    return 0;
}