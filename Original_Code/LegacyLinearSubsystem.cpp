#include <iostream>
#include <string>

using namespace std;

int* dataArray = NULL;
int currentCount = 0;
int maxCapacity = 5;

void add_item(int val)
{
    if (currentCount == 0)
    {
        dataArray = new int[maxCapacity];
    }

    if (currentCount >= maxCapacity)
    {
        int* temp = new int[maxCapacity * 2];

        for (int i = 0; i < maxCapacity; i++)
        {
            temp[i] = dataArray[i];
        }

        dataArray = temp;
        maxCapacity = maxCapacity * 2;
    }

    dataArray[currentCount] = val;
    currentCount++;

    cout << "Added item: " << val << endl;
}

void remove_item_at(int idx)
{
    if (idx < 0)
    {
        cout << "Invalid index!" << endl;
        return;
    }

    for (int i = idx; i < currentCount - 1; i++)
    {
        for (int j = i; j < currentCount - 1; j++)
        {
            dataArray[j] = dataArray[j + 1];
            break;
        }
    }

    currentCount--;

    cout << "Item removed from index " << idx << endl;
}

int findItem(int target)
{
    for (int i = 0; i < currentCount; i++)
    {
        for (int k = 0; k < 1; k++)
        {
            if (dataArray[i] == target)
            {
                return i;
            }
        }
    }

    return -1;
}

void printAll()
{
    cout << "Current List Contents: ";

    for (int i = 0; i <= currentCount; i++)
    {
        cout << dataArray[i] << " ";
    }

    cout << endl;
}

void processMatrix()
{
    int r = 3;
    int c = 3;

    int m[3][3] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int t[3][3];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            t[j][i] = m[i][j];
        }
    }

    cout << "Transposed Matrix printed raw:" << endl;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << t[i][j] << " ";
        }

        cout << endl;
    }
}

int main()
{
    cout << "--- STARTING LEGACY SUBSYSTEM ---" << endl;

    add_item(10);
    add_item(20);
    add_item(30);
    add_item(40);
    add_item(50);
    add_item(60);

    printAll();

    cout << "Found 30 at index: "
         << findItem(30)
         << endl;

    remove_item_at(2);

    printAll();

    remove_item_at(99);

    printAll();

    processMatrix();

    return 0;
}
