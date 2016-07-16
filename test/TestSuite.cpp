#include <iostream>
#include <string>

#include <io/System.h>
#include <util/String.h>
#include <test/Assert.h>
#include <memory/UniquePtr.h>
#include <container/ArrayList.h>
#include <container/LinkedList.h>

using namespace bkstl::Assert;

template <typename X>
struct MyDeleter
{
    void operator()(X* raw)
    {
        bkstl::System::out::println("Trying out custom deleter object.");
        delete raw;
    }
};

template <typename X>
void deleterFunction(X* raw)
{
    bkstl::System::out::println("Trying out custom deleter function.");
    delete raw;
}


void testUniquePtr()
{
    // Base unique pointer facilities
    bkstl::UniquePtr<int> int_ptr;
    if (!int_ptr)
    {
        int_ptr.reset(new int(100));
    }
    assertEquals(100, *int_ptr);

    // Check for no storage size overhead
    int* raw_intptr = new int(100);
    bkstl::UniquePtr<int> int_ptr2(new int(100));
    assertEquals(sizeof(raw_intptr), sizeof(int_ptr2), "UniquePtr has size overhead");
    delete raw_intptr;

    // Using custom deleter
    bkstl::UniquePtr<int, MyDeleter<int> > ptr_with_deleter(new int(55));
    assertEquals(55, *ptr_with_deleter);
    bkstl::UniquePtr<std::string, void(*)(std::string*) > strptr_with_deleter(new std::string("word"), deleterFunction<std::string>);
    assertEquals("word", *strptr_with_deleter);
}

void testArrayList()
{
    bkstl::ArrayList<int> list(5);
    assertEquals(0, list.size());
    list.add(35);
    assertEquals(35, list[0]);
    assertEquals(1, list.size());
    assertEquals(5, list.capacity());
    for (int i = 1; i <= 4; ++i)
    {
        list.add(7);
    }
    assertEquals(5, list.size());
    assertEquals(5, list.capacity());
    list.add(1);
    assertEquals(6, list.size());
    assertEquals(1005, list.capacity());

    list.clear();
    assertEquals(0, list.size());

    bkstl::UniquePtr< bkstl::List<int> > array_list(new bkstl::ArrayList<int>());
    // UniquePtr< List<int> > array_list = makeUnique<ArrayList <int> >();  // FIXME: doesn't support polymorphism yet
    array_list->add(2);
    array_list->add(34);

    assertEquals(2, array_list->get(0));
    assertEquals(34, array_list->get(1));
    assertEquals(2, array_list->size());
}

void testLinkedList()
{
    bkstl::LinkedList<int> list;

    // Initial insert
    list.add(40);
    assertEquals(40, list[0]);

    // Clear
    list.clear();
    assertEquals(0, list.size());

    // Continous insert
    int samples[] = {87, 12, 33, 65, 77};
    for (int i = 0; i < (sizeof(samples) / sizeof(samples[0])); ++i)
    {
        list.add(samples[i]);
    }
    for (int i = 0; i < (sizeof(samples) / sizeof(samples[0])); ++i)
    {
        assertEquals(samples[i], list[i]);
    }
}

void testSystemOut()
{
    using namespace bkstl;

    // const char*
    System::out::print("Test char* cout.\n");
    System::out::println("Test char* cout.");
    System::err::print("Test char* cerr.\n");
    System::err::println("Test char* cerr.");

    // const std::string&
    System::out::print(std::string("Test std::string cout.\n"));
    System::out::println(std::string("Test std::string cout."));
    System::err::print(std::string("Test std::string cerr.\n"));
    System::err::println(std::string("Test std::string cerr."));
}

int main()
{
    testSystemOut();
    testUniquePtr();
    testArrayList();
    testLinkedList();

    return 0;
}
