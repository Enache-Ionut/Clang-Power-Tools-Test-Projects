#include "stdafx.h"
#include "CustomAllocator.h"

#define MAX_MEMORY 30000
#define MAX_NUMBER_MEM_BLOCKS 60
//----------------------------------------------------------------------------

int consoleWidth;
int nextX = 0;
int nextY = 0;

class MemoryBlock
{
private:
	bool   memoryCreated{ false };
  void * startMemAddress;

  static HWND console
  static HDC  hdc;

  struct address_compare
  {		
																															bool operator()(const std::pair<size_t, void *> & lhs,
                    const std::pair<size_t, void *> & rhs) const
    {
      auto [length_pr1, pointer_pr1] = lhs;
      auto [length_pr2, pointer_pr2] = rhs;

      if (length_pr1 == length_pr2)
      {
        return static_cast<char *>(pointer_pr1) < static_cast<char *>(pointer_pr2);
      }
      {
        return length_pr1 < length_pr2;
      }
    }
  };

  std::multiset<std::pair<size_t, void *>, address_compare> startingAddresses;

  std::map<char *, size_t> occupiedAddresses;

  std::map<char *, size_t> snapShotOccupiedAddresses;

  int xLeft{ 1 }, yLeft{ 0 };

  void drawRange(const char * key, size_t length, COLORREF COLOR)
  {
    const int MAX_SQRT = static_cast<int>(sqrt(MAX_MEMORY));

    if (static_cast<int>(length) < MAX_SQRT)
    {
      for (size_t i = key - static_cast<char *>(startMemAddress),
                  j = static_cast<size_t>(i / MAX_SQRT) + yLeft;
           i < key - static_cast<char *>(startMemAddress) + length; i++)
      {
        SetPixel(hdc, xLeft + (i + 1) % MAX_SQRT, static_cast<int>(j), COLOR);
        j += static_cast<unsigned long long>((i + 1) % MAX_SQRT == 0);
      }
    }
    else
    {
      size_t i, j;
      for (i = key - static_cast<char *>(startMemAddress),
          j  = static_cast<size_t>(i / MAX_SQRT) + yLeft;
           ; i++)
      {
        SetPixel(hdc, xLeft + (static_cast<int>(i) + 1) % MAX_SQRT, static_cast<int>(j), COLOR);
        length--;
        if ((i + 1) % MAX_SQRT == 0)
        {
          break;
        }
      }
      j++;

      RECT   myRect  = { xLeft, static_cast<int>(j), xLeft + MAX_SQRT,
                      static_cast<int>(j) + static_cast<int>(length) / MAX_SQRT };
      HBRUSH handler = CreateSolidBrush(COLOR);
      FillRect(hdc, &myRect, handler);
      DeleteObject(handler);

      j += static_cast<int>(length) / MAX_SQRT;
      length -= MAX_SQRT * (static_cast<int>(length) / MAX_SQRT);

      int k = 1;
      while ((length--) != 0u)
      {
        SetPixel(hdc, xLeft + k++, static_cast<int>(j), COLOR);
      }
    }
  }

  void clearVisualiserArea()
  {
    const int MAX_SQRT = static_cast<int>(sqrt(MAX_MEMORY));
    COLORREF  COLOR    = RGB(0, 139, 139);

    // Draw pixels
    RECT   myRect  = { xLeft, yLeft, xLeft + MAX_SQRT, MAX_SQRT + yLeft + 1 };
    HBRUSH handler = CreateSolidBrush(COLOR);
    FillRect(hdc, &myRect, handler);

    DeleteObject(handler);

    COLOR = RGB(255, 255, 102);

    myRect = { xLeft + MAX_SQRT, yLeft, xLeft + MAX_SQRT + 1, MAX_SQRT + yLeft + 1 };

    handler = CreateSolidBrush(COLOR);
    FillRect(hdc, &myRect, handler);

    DeleteObject(handler);

    COLOR = RGB(255, 255, 102);

    myRect = { xLeft, yLeft + MAX_SQRT + 1, xLeft + MAX_SQRT + 1, yLeft + MAX_SQRT + 2 };

    handler = CreateSolidBrush(COLOR);
    FillRect(hdc, &myRect, handler);

    DeleteObject(handler);

    COLOR = RGB(255, 255, 102);

    myRect = { xLeft, yLeft - 1, xLeft + MAX_SQRT + 1, yLeft + 1 };

    handler = CreateSolidBrush(COLOR);
    FillRect(hdc, &myRect, handler);

    DeleteObject(handler);

    COLOR = RGB(255, 255, 102);

    myRect = { xLeft - 1, yLeft, xLeft, yLeft + MAX_SQRT + 1 };

    handler = CreateSolidBrush(COLOR);
    FillRect(hdc, &myRect, handler);

    DeleteObject(handler);
  }

public:
  MemoryBlock() = default;

  void setVisualiserCoordinates(int x, int y)
  {
    xLeft = x;
    yLeft = y;

    if (hdc != nullptr)
    {
      clearVisualiserArea();
    }
  }

  void * malloc(size_t aSize)
  {
    // default CRT implementation
    void * ptrMem = nullptr;

    if (!memoryCreated)
    {
      ptrMem          = GlobalAlloc(GMEM_FIXED, static_cast<size_t>(MAX_MEMORY));
      startMemAddress = ptrMem;

      memoryCreated = true;

      // memset((char*)ptrMem, 0, (size_t)MAX_MEMORY);

      startingAddresses.insert({ MAX_MEMORY - aSize, static_cast<char *>(ptrMem) + aSize });

      memset(ptrMem, 0, MAX_MEMORY);

      occupiedAddresses[static_cast<char *>(ptrMem)] = aSize;

      if (hdc != nullptr)
      {
        drawRange(static_cast<char *>(ptrMem), aSize, RGB(139, 0, 0));
      }

      return static_cast<char *>(ptrMem);
    }

    auto iteratorAddress = startingAddresses.lower_bound({ aSize, (void *)nullptr });

    if (iteratorAddress == startingAddresses.end())
    {
      return ptrMem;  // nullptr
    }

    auto [lengthAddress, pointerAddress] = *iteratorAddress;
    startingAddresses.erase(iteratorAddress);

    if (lengthAddress - aSize > 0)
    {
      startingAddresses.insert(
        { lengthAddress - aSize, static_cast<char *>(pointerAddress) + aSize });
    }

    ptrMem = static_cast<char *>(pointerAddress);

    occupiedAddresses[static_cast<char *>(ptrMem)] = aSize;

    if (hdc != nullptr)
    {
      drawRange(static_cast<char *>(ptrMem), aSize, RGB(139, 0, 0));
    }

    return ptrMem;
  }

  bool free(void * aBlock)
  {
    auto location = occupiedAddresses.find(static_cast<char *>(aBlock));

    if (location == end(occupiedAddresses))
    {
      return false;
    }
    size_t length = (*location).second;

    if (hdc != nullptr)
    {
      drawRange((*location).first, length, RGB(0, 139, 139));
    }

    auto   prev_loc     = location;
    void * startAddress = nullptr;
    location++;
    if (location != end(occupiedAddresses))
    {
      if ((*location).first - ((*prev_loc).first + (*prev_loc).second) > 0)
      {
        startingAddresses.erase({ (*location).first - ((*prev_loc).first + (*prev_loc).second),
                                  (*prev_loc).first + (*prev_loc).second });
      }
      length += (*location).first - ((*prev_loc).first + (*prev_loc).second);
    }
    else
    {
      length += MAX_MEMORY -
                ((*prev_loc).first - static_cast<char *>(startMemAddress) + (*prev_loc).second);
      if (MAX_MEMORY -
            ((*prev_loc).first - static_cast<char *>(startMemAddress) + (*prev_loc).second) !=
          0)
      {
        startingAddresses.erase(
          { MAX_MEMORY -
              ((*prev_loc).first - static_cast<char *>(startMemAddress) + (*prev_loc).second),
            (*prev_loc).first + (*prev_loc).second });
      }
    }

    location--;

    if (location != begin(occupiedAddresses))
    {
      location--;
      if ((*prev_loc).first - ((*location).first + (*location).second) > 0)
      {
        startingAddresses.erase({ (*prev_loc).first - ((*location).first + (*location).second),
                                  ((*location).first + (*location).second) });
      }
      length += (*prev_loc).first - ((*location).first + (*location).second);
      startAddress = ((*location).first + (*location).second);
    }
    else
    {
      length += (*prev_loc).first - static_cast<char *>(startMemAddress);
      startAddress = startMemAddress;

      if (length != 0)
      {
        startingAddresses.erase(
          { (*prev_loc).first - static_cast<char *>(startMemAddress), startMemAddress });
      }
    }

    occupiedAddresses.erase(prev_loc);

    startingAddresses.insert({ length, startAddress });

    return true;
  }

  void begin_SnapShot()
  {
    // snapShotOccupiedAddresses.clear();

    snapShotOccupiedAddresses.insert(begin(occupiedAddresses), end(occupiedAddresses));
  }

  bool end_SnapShot()
  {
    for (auto [key, value] : occupiedAddresses)
    {
      if (snapShotOccupiedAddresses.find(key) == snapShotOccupiedAddresses.end())
      {
        return 1;
      }
    }
    return 0;
  }

  size_t memory_Usage()
  {
    size_t sum = 0;
    for (const auto & [key, length] : occupiedAddresses)
    {
      sum += length;
    }

    return sum;
    // std::cout << std::setprecision(6) << std::fixed << "Memory used: %" << (sum / MAX_MEMORY *
    // 100) << '\n';
  }

  size_t max_Available()
  {
    if (!startingAddresses.empty())
    {
      return rbegin(startingAddresses)->first;
    }
    {
      return 0;
    }
  }

  double metric_Fragmentation()
  {
    return 1 - (4 *
                pow((static_cast<long long>(startingAddresses.size()) -
                     static_cast<long long>(MAX_MEMORY) / 2),
                    2) /
                static_cast<double>(MAX_MEMORY) / static_cast<double>(MAX_MEMORY)) *
                 (static_cast<double>(memory_Usage()) / (MAX_MEMORY - max_Available()));
  }

  friend void memoryVisualise();
};

HWND MemoryBlock::console = nullptr;
HDC  MemoryBlock ::hdc    = nullptr;

std::list<MemoryBlock> memoryBlocks;

void * __cdecl CustomAllocator_New(size_t       aSize,
                                   int          aBlockUse,
                                   char const * aFileName,
                                   int          aLineNumber)
{
  return CustomAllocator_Malloc(aSize, aBlockUse, aFileName, aLineNumber);
}

void __cdecl CustomAllocator_Delete(void *       aBlock,
                                    int          aBlockUse,
                                    char const * aFileName,
                                    int          aLineNumber) noexcept
{
  CustomAllocator_Free(aBlock, aBlockUse, aFileName, aLineNumber);
}

void * __cdecl CustomAllocator_Malloc(size_t aSize,
                                      int /* aBlockUse*/,
                                      char const * /*aFileName*/,
                                      int /*aLineNumber*/)
{
  for (auto & memBlock : memoryBlocks)
  {
    void * memAddress = memBlock.malloc(aSize);

    if (memAddress != nullptr)
    {
      return memAddress;
    }
  }

  if (memoryBlocks.size() < MAX_NUMBER_MEM_BLOCKS)
  {
    memoryBlocks.emplace_back();

    if (nextX + sqrt(MAX_MEMORY) + 1 < consoleWidth)
    {
      memoryBlocks.back().setVisualiserCoordinates(nextX + 1, nextY + 50);

      nextX += static_cast<int>(sqrt(MAX_MEMORY) + 1) + 1;
    }
    else
    {
      nextX = 0;
      nextY += static_cast<int>(sqrt(MAX_MEMORY)) + 3;
      memoryBlocks.back().setVisualiserCoordinates(nextX + 1, nextY + 50);
      nextX += static_cast<int>(sqrt(MAX_MEMORY) + 1) + 1;
    }

    return memoryBlocks.back().malloc(aSize);
  }
  {
    return nullptr;
  }

  // return _malloc_dbg(aSize, aBlockUse, aFileName, aLineNumber);
}

void __cdecl CustomAllocator_Free(void * aBlock,
                                  int /*aBlockUse*/,
                                  char const * /*aFileName*/,
                                  int /*aLineNumber*/)
{
  for (auto & memBlock : memoryBlocks)
  {
    memBlock.free(aBlock);
  }
  // default CRT implementation
  // GlobalFree(aBlock);
}
void _cdecl memoryVisualise()
{
  // Get a console handle
  MemoryBlock::console = GetConsoleWindow();
  // Get a handle to device context
  MemoryBlock::hdc = GetDC(MemoryBlock::console);

  RECT rect;

  GetWindowRect(MemoryBlock::console, &rect);

  consoleWidth = rect.right;
}

void _cdecl beginSnapShot()
{
  // snapShotOccupiedAddresses.clear();

  for (auto & memBlock : memoryBlocks)
  {
    memBlock.begin_SnapShot();
  }
}

bool _cdecl endSnapShot()
{
  for (auto & memBlock : memoryBlocks)
  {
    if (static_cast<int>(memBlock.end_SnapShot()) == 1)
    {
      return 1;
    }
  }
  return 0;
}

size_t _cdecl memoryUsage()
{
  size_t sum = 0;
  for (auto & memBlock : memoryBlocks)
  {
    sum += memBlock.memory_Usage();
  }

  return sum;
  // std::cout << std::setprecision(6) << std::fixed << "Memory used: %" << (sum / MAX_MEMORY * 100)
  // << '\n';
}

double _cdecl metricFragmentation(int index)
{
  if (index < static_cast<int>(memoryBlocks.size()))
  {
    auto it = begin(memoryBlocks);

    advance(it, index);

    return (*it).metric_Fragmentation();
  }
  {
    return -1;
  }
}

size_t _cdecl maxAvailable()
{
  size_t mx = 0;

  for (auto & memBlock : memoryBlocks)
  {
    mx = max(mx, memBlock.max_Available());
  }

  return mx;
}
