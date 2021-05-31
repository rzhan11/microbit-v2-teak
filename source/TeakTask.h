
#define MICROBIT_ID_TIMER 512

// class TeakTask {
// public:
//     int m_image;
//     bool m_asyncImage;
//     TeakTask* m_leftTask;
//     TeakTask* m_rightTask;
//     int m_note;
//
// public:
//     TeakTask();
//     void SetAdjacentTasks(TeakTask* left, TeakTask* right);
//     int PackedImage() { return m_image; };
//     bool AsyncImage() { return m_asyncImage; };
//     virtual void Event(MicroBitEvent) { return; };
// };
//
// class TeakTaskManager {
// public:
//     TeakTaskManager();
//     void Setup();
//     void MicrobitDalEvent(MicroBitEvent event);
//     void MicrobitBtEvent(MicroBitEvent event);
//     void calibrate();
//     // void SwitchTo(TeakTask* task);
//     char* BotName() { return m_name; };
//     bool m_animating;
//     bool m_btConnected = false;
// private:
//     TeakTask* m_currentTask;
//     char m_name[8];
//     int m_currentImage = 0;
// };

// bool m_animating;
void calibrate();
void MicrobitBtEvent(const char* str);
