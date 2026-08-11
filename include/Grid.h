#ifndef GRID_H
#define GRID_H

// a template helper class i made to handle the 2D arrays ill be using in MapGrid
template <typename T>
class Grid {
private:
    T** cells;
    int width;
    int height;

public:
// honestly its all simple DMA stuff here 
    // intialziation
    Grid(int w, int h) : width(w), height(h) {
        cells = new T*[width];
        for(int i = 0; i < width; i++) {
            cells[i] = new T[height];
            for(int j = 0; j < height; j++) {
                cells[i][j] = nullptr; 
            }
        }
    }

    // destruction
    ~Grid() {
        for(int i = 0; i < width; i++) {
            delete[] cells[i];
        }
        delete[] cells;
    }

    // setting if within bounds
    void set(int x, int y, T item) {
        if(x >= 0 && x < width && y >= 0 && y < height) {
            cells[x][y] = item;
        }
    }

    // getting if within bounds
    T get(int x, int y) const {
        if(x >= 0 && x < width && y >= 0 && y < height) {
            return cells[x][y];
        }
        return nullptr;
    }
    

    // getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif


// blehh