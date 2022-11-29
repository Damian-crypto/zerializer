#include <iostream>
#include "zerializer.hpp"

typedef struct Person
{
    char Name[100];
    int Age;
    char Gender;
} Person;

int main()
{
    Person p1 = {
        .Name = "John",
        .Age = 40,
        .Gender = 'M'
    };

    zeus::Zerializer zerializer;
    zeus::SerialInfo info;
    info.SerializationType = zeus::SerialType::XML;

    zerializer.Start("person_data.dat", info);
    zerializer.InsertHeader("Person-1");
    zerializer.Serialize("Name", p1.Name);
    zerializer.Serialize("Age", p1.Age);
    zerializer.Serialize("Gender", p1.Gender);
    zerializer.End();

    return 0;
}
