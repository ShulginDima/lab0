#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>

class Employee
{
private:
  int _id = 0;
  std::string _surname;
  std::string _number;
  std::string _profession;
  int _salary = 0;

public:
  // getters
  int id() { return _id; }
  const std::string & surname() { return _surname; }
  const std::string & number() { return _number; }
  int salary() { return _salary; }
  const std::string & profession() { return _profession; }

  Employee(int id, std::string surname, std::string number, int salary, std::string profession) :
    _id(id), _surname(surname), _number(number), _salary(salary), _profession(profession) {}

  Employee(std::ifstream &in) { read_from_file(in); }

  Employee()
  {
    std::cout << "input id: ";
    std::cin >> _id;
    std::cout << "input surname: ";
    std::cin >> _surname;
    std::cout << "input phone number: ";
    std::cin >> _number;
    std::cout << "input salary: ";
    std::cin >> _salary;
    std::cout << "input profession: ";
    std::cin >> _profession;
  }

  void read_from_file(std::ifstream &in)
  {
    in.read((char *)&_id, sizeof(int));
    in.read((char *)&_salary, sizeof(int));
    std::size_t lenght;
    in.read((char *)&lenght, sizeof(std::size_t));
    _profession.resize(lenght);
    in.read(_profession.data(), lenght);
    in.read((char *)&lenght, sizeof(std::size_t));
    _surname.resize(lenght);
    in.read(_surname.data(), lenght);
    in.read((char *)&lenght, sizeof(std::size_t));
    _number.resize(lenght);
    in.read(_number.data(), lenght);
  }

  void save_to_file(std::ofstream &out)
  {
    out.write((char *)&_id, sizeof(int));
    out.write((char *)&_salary, sizeof(int));
    std::size_t lenght;
    lenght = _profession.length();
    out.write((char *)&lenght, sizeof(std::size_t));
    out.write(_profession.data(), lenght);
    lenght = _surname.length();
    out.write((char *)&lenght, sizeof(std::size_t));
    out.write(_surname.data(), lenght);
    lenght = _number.length();
    out.write((char *)&lenght, sizeof(std::size_t));
    out.write(_number.data(), lenght);
  }

  void print()
  {
    std::cout << "Surname: " << _surname << "\n" <<
                 "Id: " << _id << "\n" <<
                 "Number: " << _number << "\n" <<
                 "Salary: " << _salary << "\n" <<
                 "Profession: " << _profession << "\n\n";
  }
};

class EmployeeDataBase
{
  std::vector<Employee> _employees;
  std::string _database_file;

public:
  EmployeeDataBase(const std::string &database_file) : _database_file(database_file)
  {
    if (!std::filesystem::exists(database_file))
    {
      std::ofstream out(database_file, std::ios_base::binary);
      std::size_t zero = 0;
      out.write((char *)&zero, sizeof(std::size_t));
    }
    else
      read_from_file(database_file);
  }

  ~EmployeeDataBase() { save_to_file(); }

  void add_employee(Employee e) { _employees.push_back(e); }

  void delete_employee() 
  {
    int id;
    std::cout << "input id to delete: ";
    std::cin >> id;
    for (int i = 0; i < _employees.size(); i++)
      if (_employees[i].id() == id)
      {
        _employees.erase(_employees.begin() + i);
        break;
      }
  }

  void search()
  {
    std::cout << "choose field. 0 - id, 1 - surname, 2 - phone number, 3 - salary, 4 - profession\n";
    int option;
    std::cin >> option;
    int value_int;
    std::string value_string;
    bool find = false;
    switch (option)
    {
    case 0:
      std::cout << "input id: ";
      std::cin >> value_int;
      for (auto &e : _employees)
        if (e.id() == value_int)
        {
          e.print();
          find = true;
          break;
        }
      break;

    case 1:
      std::cout << "input surname: ";
      std::cin >> value_string;
      for (auto &e : _employees)
        if (e.surname() == value_string)
        {
          e.print();
          find = true;
          break;
        }
      break;

    case 2:
      std::cout << "input phone number: ";
      std::cin >> value_string;
      for (auto &e : _employees)
        if (e.number() == value_string)
        {
          e.print();
          find = true;
          break;
        }
      break;

    case 3:
      std::cout << "input salary: ";
      std::cin >> value_int;
      for (auto &e : _employees)
        if (e.salary() == value_int)
        {
          e.print();
          find = true;
        }
      break;

    case 4:
      std::cout << "input profession: ";
      std::cin >> value_string;
      for (auto &e : _employees)
        if (e.profession() == value_string)
        {
          e.print();
          find = true;
        }
      break;

    default:
      std::cout << "invalid option number\n";
      break;
    }

    if (!find)
      std::cout << "there arent any employees\n";
  }

  void print()
  {
    for (auto &e : _employees)
      e.print();
  }

private:
  void read_from_file(const std::string &database_file)
  {
    std::size_t size;
    std::ifstream in(database_file, std::ios_base::binary);
    in.read((char *)(&size), sizeof(std::size_t));
    for (int i = 0; i < size; i++)
      _employees.push_back(Employee(in));
  }

  void save_to_file()
  {
    std::ofstream out(_database_file, std::ios_base::binary);
    std::size_t size = _employees.size();
    out.write((char *)(&size), sizeof(std::size_t));
    for (auto &e : _employees)
      e.save_to_file(out);
  }
};

int main()
{
  EmployeeDataBase db("db.txt");
  //db.add_employee(Employee(0, "bebra", "3047", 47000, "Loader"));
  //db.add_employee(Employee(47, "vitalya", "89372558521", 300000, "Cashier"));
  //db.add_employee(Employee(12, "dima", "89217471759", 1000000, "Deliveryman"));
  //db.add_employee(Employee());

  // db.delete_employee();
  db.search();
  // db.add_employee(Employee());
  //db.print();
}