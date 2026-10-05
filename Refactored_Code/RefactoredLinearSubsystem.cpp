#include <array>
#include <cstddef>
#include <iostream>
#include <stdexcept>

/*
    NATIONAL TEACHERS COLLEGE
    School of Arts, Sciences, and Technology
    Bachelor of Science in Information Technology

    Course Code : NTC_CC105
    Course      : Data Structure w/ Lab
    Activity    : Week 5 Laboratory Activity
    Topic       : Linear Architecture Synthesis & Peer Code Audit

    File        : RefactoredLinearSubsystem.cpp

    Description:
    This program is the refactored version of the legacy linear
    data subsystem. It preserves the required functionality while
    correcting memory-management, bounds-checking, algorithmic,
    and structural defects.

    Major improvements:
    - Removed mutable global variables.
    - Encapsulated dynamic array data inside a class.
    - Implemented RAII-based memory management.
    - Added safe dynamic array resizing.
    - Added strict bounds checking.
    - Removed redundant nested loops.
    - Corrected the off-by-one traversal error.
    - Added safe empty-array handling.
    - Improved naming consistency.
    - Separated matrix processing from array management.
*/


// ============================================================================
// DYNAMIC ARRAY CLASS
// ============================================================================

class DynamicArray
{
private:

    // Pointer that owns the dynamically allocated integer array.
    int* data_;

    // Number of elements currently stored in the array.
    std::size_t size_;

    // Total number of elements that can be stored before resizing.
    std::size_t capacity_;

    // Initial capacity used when no capacity is provided.
    static constexpr std::size_t DEFAULT_CAPACITY = 5;


    /*
        resize()

        Increases the array capacity when the current array becomes full.

        Process:
        1. Allocate a new, larger array.
        2. Copy all existing elements.
        3. Release the old memory.
        4. Assign the new memory to data_.
        5. Update the capacity.

        Time Complexity: O(n)
        Space Complexity: O(n)
    */
    void resize()
    {
        const std::size_t newCapacity = capacity_ * 2;

        int* newData = new int[newCapacity];

        // Copy only the elements that are currently being used.
        for (std::size_t i = 0; i < size_; ++i)
        {
            newData[i] = data_[i];
        }

        /*
            The legacy implementation omitted delete[] here.

            Deleting the old buffer before replacing the pointer prevents
            the memory leak that occurred during every dynamic resize.
        */
        delete[] data_;

        data_ = newData;
        capacity_ = newCapacity;
    }


    /*
        validateIndex()

        Ensures that an index refers to an existing element.

        Valid range:

            0 <= index < size_

        An exception is thrown when the index is invalid.

        Time Complexity: O(1)
    */
    void validateIndex(std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range(
                "Index is outside the valid array range."
            );
        }
    }


public:

    /*
        Constructor

        Creates the dynamic array and allocates its initial memory.

        This establishes ownership of the allocated resource.
    */
    explicit DynamicArray(
        std::size_t initialCapacity = DEFAULT_CAPACITY
    )
        : data_(nullptr),
          size_(0),
          capacity_(initialCapacity)
    {
        if (initialCapacity == 0)
        {
            throw std::invalid_argument(
                "Initial capacity must be greater than zero."
            );
        }

        data_ = new int[capacity_];
    }


    /*
        Destructor

        Automatically releases the dynamically allocated array.

        This is an important RAII feature because the memory is released
        automatically when the DynamicArray object goes out of scope.
    */
    ~DynamicArray()
    {
        delete[] data_;
        data_ = nullptr;
    }


    /*
        Copy construction is disabled.

        Because this class directly owns a raw pointer, allowing a
        default shallow copy could cause two objects to point to the
        same memory and eventually perform a double deletion.
    */
    DynamicArray(const DynamicArray&) = delete;


    /*
        Copy assignment is disabled for the same ownership reason.
    */
    DynamicArray& operator=(const DynamicArray&) = delete;


    /*
        add()

        Adds a new integer to the end of the array.

        If the array is full, resize() is called before insertion.

        Average / Amortized Time: O(1)
        Worst Case Time: O(n)
        Space Complexity: O(n)
    */
    void add(int value)
    {
        if (size_ == capacity_)
        {
            resize();
        }

        data_[size_] = value;
        ++size_;

        std::cout
            << "Added item: "
            << value
            << '\n';
    }


    /*
        removeAt()

        Removes an element from the specified index.

        All elements located after the removed element are shifted
        one position toward the beginning of the array.

        Example:

            Before:
            [10, 20, 30, 40]

            removeAt(1)

            After:
            [10, 30, 40]

        Best Case: O(1), when removing the final element.
        Worst Case: O(n), when removing near the beginning.
        Space: O(1)
    */
    void removeAt(std::size_t index)
    {
        // Reject invalid indexes before accessing memory.
        validateIndex(index);

        /*
            Shift each element one position to the left.

            This replaces the redundant nested loop used by the
            legacy implementation.
        */
        for (std::size_t i = index; i + 1 < size_; ++i)
        {
            data_[i] = data_[i + 1];
        }

        --size_;

        std::cout
            << "Item removed from index "
            << index
            << '\n';
    }


    /*
        find()

        Performs a standard linear search.

        Returns:
            - The index of the first matching value.
            - -1 when the value is not found.

        Best Case: O(1)
        Average Case: O(n)
        Worst Case: O(n)
        Space Complexity: O(1)
    */
    int find(int target) const
    {
        for (std::size_t i = 0; i < size_; ++i)
        {
            if (data_[i] == target)
            {
                return static_cast<int>(i);
            }
        }

        return -1;
    }


    /*
        at()

        Returns the value stored at a specified index.

        Bounds validation is performed before accessing the array.

        Time Complexity: O(1)
    */
    int at(std::size_t index) const
    {
        validateIndex(index);

        return data_[index];
    }


    /*
        print()

        Displays all currently stored elements.

        The loop uses:

            i < size_

        instead of:

            i <= size_

        This corrects the off-by-one error found in the legacy
        printAll() function.

        Time Complexity: O(n)
        Space Complexity: O(1)
    */
    void print() const
    {
        std::cout << "Current List Contents: ";

        if (size_ == 0)
        {
            std::cout << "[empty]\n";
            return;
        }

        for (std::size_t i = 0; i < size_; ++i)
        {
            std::cout << data_[i] << ' ';
        }

        std::cout << '\n';
    }


    /*
        clear()

        Removes all logical elements from the array.

        The allocated capacity remains available for future insertions.

        Time Complexity: O(1)
    */
    void clear() noexcept
    {
        size_ = 0;
    }


    /*
        getSize()

        Returns the number of currently stored elements.
    */
    std::size_t getSize() const noexcept
    {
        return size_;
    }


    /*
        getCapacity()

        Returns the currently allocated capacity.
    */
    std::size_t getCapacity() const noexcept
    {
        return capacity_;
    }


    /*
        empty()

        Returns true when the array contains no elements.
    */
    bool empty() const noexcept
    {
        return size_ == 0;
    }
};


// ============================================================================
// MATRIX PROCESSOR
// ============================================================================

class MatrixProcessor
{
private:

    // The laboratory matrix uses a fixed 3 x 3 structure.
    static constexpr std::size_t ROWS = 3;
    static constexpr std::size_t COLS = 3;

public:

    /*
        Matrix type.

        std::array provides a safer fixed-size matrix representation
        than a raw built-in array.
    */
    using Matrix =
        std::array<std::array<int, COLS>, ROWS>;


    /*
        transpose()

        Creates and returns the transpose of the supplied matrix.

        Transposition rule:

            result[column][row] = matrix[row][column]

        Time Complexity: O(R * C)
        Space Complexity: O(R * C)
    */
    static Matrix transpose(const Matrix& matrix)
    {
        Matrix result{};

        for (std::size_t row = 0; row < ROWS; ++row)
        {
            for (std::size_t column = 0;
                 column < COLS;
                 ++column)
            {
                result[column][row] =
                    matrix[row][column];
            }
        }

        return result;
    }


    /*
        print()

        Displays a matrix one row at a time.
    */
    static void print(const Matrix& matrix)
    {
        for (const auto& row : matrix)
        {
            for (const int value : row)
            {
                std::cout << value << ' ';
            }

            std::cout << '\n';
        }
    }
};


// ============================================================================
// TESTING FUNCTIONS
// ============================================================================

/*
    testBasicInsertion()

    Tests insertion within the initial capacity.
*/
void testBasicInsertion()
{
    std::cout
        << "\n--- TC-01: BASIC INSERTION ---\n";

    DynamicArray records;

    records.add(10);
    records.add(20);
    records.add(30);

    const bool passed =
        records.getSize() == 3 &&
        records.at(0) == 10 &&
        records.at(1) == 20 &&
        records.at(2) == 30;

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << '\n';

    records.print();
}


/*
    testDynamicResize()

    Adds six elements to an array with an initial capacity of five.

    The sixth insertion forces the capacity to double.
*/
void testDynamicResize()
{
    std::cout
        << "\n--- TC-02: DYNAMIC RESIZING ---\n";

    DynamicArray records(5);

    records.add(10);
    records.add(20);
    records.add(30);
    records.add(40);
    records.add(50);

    const std::size_t oldCapacity =
        records.getCapacity();

    records.add(60);

    const bool passed =
        records.getCapacity() == oldCapacity * 2 &&
        records.getSize() == 6 &&
        records.at(5) == 60;

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << '\n';

    std::cout
        << "Old Capacity: "
        << oldCapacity
        << '\n';

    std::cout
        << "New Capacity: "
        << records.getCapacity()
        << '\n';

    records.print();
}


/*
    testSearch()

    Tests both successful and unsuccessful linear searches.
*/
void testSearch()
{
    std::cout
        << "\n--- TC-04/TC-05: LINEAR SEARCH ---\n";

    DynamicArray records;

    records.add(10);
    records.add(20);
    records.add(30);
    records.add(40);

    const int existingResult =
        records.find(30);

    const int missingResult =
        records.find(999);

    const bool passed =
        existingResult == 2 &&
        missingResult == -1;

    std::cout
        << "Search for 30: index "
        << existingResult
        << '\n';

    std::cout
        << "Search for 999: index "
        << missingResult
        << '\n';

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << '\n';
}


/*
    testDeletion()

    Tests valid deletion and verifies that the remaining elements
    are shifted correctly.
*/
void testDeletion()
{
    std::cout
        << "\n--- TC-06: ELEMENT DELETION ---\n";

    DynamicArray records;

    records.add(10);
    records.add(20);
    records.add(30);
    records.add(40);

    records.removeAt(2);

    const bool passed =
        records.getSize() == 3 &&
        records.at(0) == 10 &&
        records.at(1) == 20 &&
        records.at(2) == 40;

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << '\n';

    records.print();
}


/*
    testInvalidIndex()

    Attempts to delete an element outside the valid array range.

    The refactored implementation must reject the operation without
    corrupting the array.
*/
void testInvalidIndex()
{
    std::cout
        << "\n--- TC-08: UPPER BOUND VALIDATION ---\n";

    DynamicArray records;

    records.add(10);
    records.add(20);
    records.add(30);

    bool exceptionCaught = false;

    try
    {
        records.removeAt(99);
    }
    catch (const std::out_of_range& exception)
    {
        exceptionCaught = true;

        std::cout
            << "Validation message: "
            << exception.what()
            << '\n';
    }

    const bool stateUnchanged =
        records.getSize() == 3;

    const bool passed =
        exceptionCaught &&
        stateUnchanged;

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << '\n';

    records.print();
}


/*
    testEmptyArray()

    Verifies that operations on an empty array are handled safely.
*/
void testEmptyArray()
{
    std::cout
        << "\n--- EMPTY STATE TEST ---\n";

    DynamicArray records;

    const bool initiallyEmpty =
        records.empty();

    records.print();

    bool deletionRejected = false;

    try
    {
        records.removeAt(0);
    }
    catch (const std::out_of_range&)
    {
        deletionRejected = true;
    }

    const bool passed =
        initiallyEmpty &&
        deletionRejected;

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << '\n';
}


/*
    processMatrix()

    Creates a 3 x 3 matrix, transposes it, and displays the result.

    Expected transposed matrix:

        1 4 7
        2 5 8
        3 6 9
*/
void processMatrix()
{
    std::cout
        << "\n--- TC-09: MATRIX TRANSPOSE ---\n";

    const MatrixProcessor::Matrix matrix =
    {{
        {{1, 2, 3}},
        {{4, 5, 6}},
        {{7, 8, 9}}
    }};

    const auto transposed =
        MatrixProcessor::transpose(matrix);

    const bool passed =
        transposed[0][0] == 1 &&
        transposed[0][1] == 4 &&
        transposed[0][2] == 7 &&
        transposed[1][0] == 2 &&
        transposed[1][1] == 5 &&
        transposed[1][2] == 8 &&
        transposed[2][0] == 3 &&
        transposed[2][1] == 6 &&
        transposed[2][2] == 9;

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << '\n';

    std::cout
        << "Transposed Matrix:\n";

    MatrixProcessor::print(transposed);
}


/*
    main()

    Runs the refactored subsystem and its validation tests.

    All DynamicArray objects automatically release their allocated
    memory when they leave scope.
*/
int main()
{
    try
    {
        std::cout
            << "====================================================\n"
            << "       NATIONAL TEACHERS COLLEGE - BSIT\n"
            << "       NTC_CC105 DATA STRUCTURE WITH LAB\n"
            << "       WEEK 5 REFACTORED LINEAR SUBSYSTEM\n"
            << "====================================================\n";

        testBasicInsertion();

        testDynamicResize();

        testSearch();

        testDeletion();

        testInvalidIndex();

        testEmptyArray();

        processMatrix();

        std::cout
            << "\n====================================================\n"
            << "       REFACTORED SUBSYSTEM COMPLETED\n"
            << "       MEMORY MANAGEMENT: RAII ENABLED\n"
            << "       BOUNDS CHECKING: ENABLED\n"
            << "       GLOBAL ARRAY STATE: REMOVED\n"
            << "       REDUNDANT LOOPS: REMOVED\n"
            << "====================================================\n";
    }
    catch (const std::exception& exception)
    {
        /*
            Handles unexpected runtime exceptions so that the program
            reports the problem instead of terminating silently.
        */
        std::cerr
            << "\nFatal error: "
            << exception.what()
            << '\n';

        return 1;
    }

    return 0;
}