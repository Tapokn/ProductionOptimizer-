#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;


struct Product {
    string name;    // Название товара
    int res1;       // Расход ресурса 1 (сырье, кг)
    int res2;       // Расход ресурса 2 (рабочее время, часы)
    int price;      // Цена единицы продукции
};


// Очистка входного буфера после ошибочного ввода. 
// Функция очищает поток ввода и удаляет остаток строки из буфера.
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
// Проверка ввода.  
//Считывает целое число из ввода и возвращает true, если в строке больше нет других символов.
bool readIntStrict(int& value) {
    if (!(cin >> value)) {
        clearInputBuffer();
        return false;
    }


    if (cin.peek() != '\n') {
        clearInputBuffer();
        return false;
    }


    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}


// Ввод целого числа с валидацией ограничений
int inputInt(const string& prompt) {
    int value;


    while (true) {
        cout << prompt;


        if (!readIntStrict(value)) {
            cout << "Ошибка: введите одно целое число без лишних символов.\n";
            continue;
        }


        if (value < 0 || value > 46340) {
            cout << "Ошибка: число должно быть в диапазоне от 0 до 46340.\n";
            continue;
        }


        return value;
    }
}


// Ввод названия товара с валидацией длины
string inputProductName(const string& prompt) {
    string name;
    while (true) {
        cout << prompt;
        getline(cin, name);         
        if (name.empty() || name.size() > 50) {
            cout << "Ошибка: название не должно превышать 50 символов.\n";
            continue;
        }
        return name;
    }
}


// Ввод количества видов продукции
int inputProductTypeCount() {
    int count;


    while (true) {
        cout << "Введите количество видов продукции (1 или 2): ";


        if (!readIntStrict(count)) {
            cout << "Ошибка: введите одно целое число без лишних символов.\n";
            continue;
        }


        if (count == 1 || count == 2) {
            return count;
        }


        cout << "Ошибка: допустимые значения 1 или 2.\n";
    }
}


// Универсальная функция расчёта максимального количества товара
// Функция вычисляет максимальное количество изделий, которое можно произвести при заданных ресурсах и расходах
int computeMaxCount(int maxRes1, int maxRes2, int res1, int res2) {
    if (res1 > 0 && res2 > 0) {
        return min(maxRes1 / res1, maxRes2 / res2);
    } else if (res1 > 0) {
        return maxRes1 / res1;
    } else { // res2 > 0
        return maxRes2 / res2;
    }
}


// Ввод данных о товаре с проверкой на доступные ресурсы
// Функция запрашивает у пользователя данные о товаре, проверяет корректность расходов ресурсов и возвращает заполненную структуру Product.


Product inputProduct(int availableRes1, int availableRes2, int productNumber) {
    Product product;
    
    cout << "\n=== Ввод данных для товара " << productNumber << " ===\n";
    
    product.name = inputProductName("Введите название товара: ");
    
    while (true) {
        cout << "\nВведите расходы ресурсов для товара '" << product.name << "':\n";
        product.res1 = inputInt("  Расход ресурса 1 (кг сырья): ");
        product.res2 = inputInt("  Расход ресурса 2 (часы работы): ");
        
        // Проверка корректности введенных данных
        if (product.res1 == 0 && product.res2 == 0) {
            cout << "Ошибка: оба расхода не могут быть нулевыми.\n";
            continue;
        }
        
        if (product.res1 > availableRes1) {
            cout << "Ошибка: расход сырья (" << product.res1 
                 << ") превышает доступное количество (" << availableRes1 << ").\n";
            continue;
        }
        
        if (product.res2 > availableRes2) {
            cout << "Ошибка: расход рабочего времени (" << product.res2 
                 << ") превышает доступное количество (" << availableRes2 << ").\n";
            continue;
        }
        
        break;
    }
    
    product.price = inputInt("Введите цену единицы продукции: ");
    
    return product;
}


// Поиск оптимальных комбинаций производства
// Функция находит оптимальные комбинации товаров для максимальной выручки и сохраняет результаты в переданные параметры bestProfit, bestCombinations, usedRes1, usedRes2.




void findOptimalCombinations(const vector<Product>& products, int maxRes1, int maxRes2,
                             int& bestProfit, vector<vector<int>>& bestCombinations,
                             vector<int>& usedRes1, vector<int>& usedRes2) {
    
    bestProfit = 0;
    bestCombinations.clear();
    usedRes1.clear();
    usedRes2.clear();
    
    // Обработка случая с одним товаром
    if (products.size() == 1) {
        const Product& p = products[0];
        
        int maxCount = computeMaxCount(maxRes1, maxRes2, p.res1, p.res2);
        
        bestProfit = maxCount * p.price;
        bestCombinations.push_back({maxCount});
        usedRes1.push_back(maxCount * p.res1);
        usedRes2.push_back(maxCount * p.res2);
        
        return;
    }
    
    // Обработка случая с двумя товарами
    const Product& p1 = products[0];
    const Product& p2 = products[1];
    
    // Определяем максимально возможное количество первого товара
    int maxCnt1 = computeMaxCount(maxRes1, maxRes2, p1.res1, p1.res2);
    
    // Перебираем все возможные количества первого товара
    for (int cnt1 = 0; cnt1 <= maxCnt1; ++cnt1) {
        int remainingRes1 = maxRes1 - cnt1 * p1.res1;
        int remainingRes2 = maxRes2 - cnt1 * p1.res2;
        
        // Определяем максимально возможное количество второго товара
        int cnt2 = computeMaxCount(remainingRes1, remainingRes2, p2.res1, p2.res2);
        
        // Вычисляем выручку для данной комбинации
        int currentProfit = cnt1 * p1.price + cnt2 * p2.price;
        
        // Обновляем лучшие результаты
        if (currentProfit > bestProfit) {
            bestProfit = currentProfit;
            bestCombinations.clear();
            usedRes1.clear();
            usedRes2.clear();
            
            bestCombinations.push_back({cnt1, cnt2});
            usedRes1.push_back(cnt1 * p1.res1 + cnt2 * p2.res1);
            usedRes2.push_back(cnt1 * p1.res2 + cnt2 * p2.res2);
        } 
        else if (currentProfit == bestProfit && currentProfit > 0) {
            bestCombinations.push_back({cnt1, cnt2});
            usedRes1.push_back(cnt1 * p1.res1 + cnt2 * p2.res1);
            usedRes2.push_back(cnt1 * p1.res2 + cnt2 * p2.res2);
        }
    }
}


// Форматирование процента использования ресурса
string formatPercentage(int used, int total) {
    if (total == 0) return "0.0%";
    
    double percentage = (static_cast<double>(used) / total) * 100;
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.1f%%", percentage);
    
    return string(buffer);
}


// Вывод результатов
void printResults(const vector<Product>& products, int bestProfit,
                  const vector<vector<int>>& bestCombinations,
                  const vector<int>& usedRes1, const vector<int>& usedRes2,
                  int maxRes1, int maxRes2) {
    
    cout << "\n" << string(50, '=') << "\n";
    cout << "РЕЗУЛЬТАТЫ ОПТИМИЗАЦИИ ПРОИЗВОДСТВА\n";
    cout << string(50, '=') << "\n\n";
    
    cout << "Лучшая выручка: " << bestProfit << "\n\n";
    cout << "Оптимальные комбинации производства:\n";
    
    for (size_t i = 0; i < bestCombinations.size(); ++i) {
        cout << "\nВариант " << (i + 1) << ":\n";
        
        if (products.size() == 1) {
            cout << "  " << products[0].name << ": " << bestCombinations[i][0] << " единиц\n";
        } else {
            cout << "  " << products[0].name << ": " << bestCombinations[i][0] << " единиц\n";
            cout << "  " << products[1].name << ": " << bestCombinations[i][1] << " единиц\n";
        }
        
        cout << "  Использование ресурсов:\n";
        cout << "    Сырье: " << usedRes1[i] << " / " << maxRes1 << " кг (" 
             << formatPercentage(usedRes1[i], maxRes1) << ")\n";
        cout << "    Рабочее время: " << usedRes2[i] << " / " << maxRes2 << " часов (" 
             << formatPercentage(usedRes2[i], maxRes2) << ")\n";
    }
    
    cout << "\n" << string(50, '=') << "\n";
}


int main() {
    setlocale(LC_ALL, "Russian");
    
    cout << "ПРОГРАММА ОПТИМИЗАЦИИ ПРОИЗВОДСТВА\n";
    cout << string(40, '-') << "\n\n";
    
    // Ввод основных параметров
    int productTypeCount = inputProductTypeCount();
    
    cout << "\nВведите доступные ресурсы:\n";
    int maxRes1 = inputInt("  Общий запас сырья (кг): ");
    int maxRes2 = inputInt("  Общий запас рабочего времени (часы): ");
   
    // Завершение программы если максимальных ресурсов 0
    if (maxRes1 == 0 && maxRes2 == 0) {
        cout << "\nУведомление: При нулевых запасах всех ресурсов производство невозможно." << endl;
        cout << "Оптимальная выручка: 0. Программа завершена." << endl;
        return 0;
    }
    
    // Подсказки для пользователя
    if (maxRes1 == 0 && maxRes2 > 0) { // Есть только время (Толлинг/Услуги)
        cout << "- В качестве единицы продукции (товара) используйте '1 нормо-час'." << endl;
        cout << "- Цену единицы продукции указывайте как вашу ставку за один час работы." << endl;
        cout << "Переименование вывода программа не предусматривает." << endl;
    } 
    
    if (maxRes2 == 0 && maxRes1 > 0) { // Есть только сырье (Продажа излишков)
        cout << "- За единицу продукции берите удобную партию (кг, центнер, тонна)." << endl;
        cout << "- В качестве цены указывайте стоимость реализации всей этой партии." << endl;
        cout << "Переименование вывода программа не предусматривает." << endl;
    }
    
    cout << "--------------------------------------------------------" << endl;


    // Ввод данных о товарах
    vector<Product> products;
    for (int i = 0; i < productTypeCount; ++i) {
        products.push_back(inputProduct(maxRes1, maxRes2, i + 1));
    }
    
    // Поиск оптимальных комбинаций
    int bestProfit;
    vector<vector<int>> bestCombinations;
    vector<int> usedRes1, usedRes2;
    
    findOptimalCombinations(products, maxRes1, maxRes2, 
                            bestProfit, bestCombinations, 
                            usedRes1, usedRes2);
    
    // Вывод результатов
    printResults(products, bestProfit, bestCombinations, 
                 usedRes1, usedRes2, maxRes1, maxRes2);
    
    return 0;
}
