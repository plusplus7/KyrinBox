#include "test.pb.h"
#include <iostream>

using namespace std;

int main()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    kyrinbox::test::Group group;
    kyrinbox::test::Person *person = group.add_person();
    person->set_name("plusplus7");
    person->set_age(21);
    person->set_email("JackieDeng.cn@gmail.com");

    person = group.add_person();
    person->set_name("sssogs");
    person->set_age(15);
    person->set_email("sssogs@qq.com");

    string group_string;

    group.SerializeToString(&group_string);

    cout << "# " << group_string << " #" <<endl;
    kyrinbox::test::Group ag;
    ag.ParseFromString(group_string);

    for (int i=0; i<ag.person_size(); i++) {
        const kyrinbox::test::Person &person = ag.person(i);
        cout << "Name: " << person.name() << endl;
        cout << "Age: " << person.age() << endl;
        if (person.has_email()) {
            cout << "E-mail: " << person.email() << endl;
        }
    }
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
