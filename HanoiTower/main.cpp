#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class Tower {
public:
    // конструктор и метод SetDisks нужны, чтобы правильно создать башни
    Tower(int disks_num) {
        FillTower(disks_num);
    }

    int GetDisksNum() const {
        return static_cast<int>(disks_.size());
    }

    void SetDisks(int disks_num) {
        FillTower(disks_num);
    }

    // добавляем диск на верх собственной башни
    // обратите внимание на исключение, которое выбрасывается этим методом
    void AddToTop(int disk) {
        int top_disk_num = static_cast<int>(disks_.size()) - 1;
        if (0 != disks_.size() && disk >= disks_[top_disk_num]) {
            throw invalid_argument("Невозможно поместить большой диск на маленький");
        } else {
            disks_.push_back(disk);
        }
    }
    
    int PopFromTop() {
        int output = disks_.back();
        disks_.pop_back();
        return output;
    }
    
    // disks_num - количество перемещаемых дисков
    // destination - конечная башня для перемещения
    // buffer - башня, которую нужно использовать в качестве буфера для дисков
    void MoveDisks(int disks_num, Tower& destination, Tower& buffer) {
        if (1 == disks_num) {
            destination.AddToTop(PopFromTop());
            return;
        }
        MoveDisks(disks_num - 1, buffer, destination);
        destination.AddToTop(PopFromTop());
        buffer.MoveDisks(disks_num - 1, destination, *this);
    }

private:
    vector<int> disks_;

    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int disks_num) {
        for (int i = disks_num; i > 0; i--) {
            disks_.push_back(i);
        }
    }
};

void SolveHanoi(vector<Tower>& towers) {
    int disks_num = towers[0].GetDisksNum();
    // запускаем рекурсию
    // просим переложить все диски на последнюю башню
    // с использованием средней башни как буфера
    towers[0].MoveDisks(disks_num, towers[2], towers[1]);
}

int main() {
    int towers_num = 3;
    int disks_num = 3;
    vector<Tower> towers;
    // добавим в вектор три пустые башни
    for (int i = 0; i < towers_num; ++i) {
        towers.push_back(0);
    }
    // добавим на первую башню три кольца
    towers[0].SetDisks(disks_num);

    SolveHanoi(towers);
    return 0;
}
