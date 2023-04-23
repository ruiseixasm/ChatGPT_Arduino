#include <Arduino.h>

class Observer {
public:
  virtual void update(int value) = 0;
};

class Subject {
public:
  void attach(Observer *observer) {
    observers_[observer_count_++] = observer;
  }

  void detach(Observer *observer) {
    for (int i = 0; i < observer_count_; i++) {
      if (observers_[i] == observer) {
        for (int j = i; j < observer_count_ - 1; j++) {
          observers_[j] = observers_[j + 1];
        }
        observer_count_--;
        break;
      }
    }
  }

  void notify(int value) {
    for (int i = 0; i < observer_count_; i++) {
      observers_[i]->update(value);
    }
  }

private:
  Observer *observers_[10];
  int observer_count_ = 0;
};

class ConcreteObserver : public Observer {
public:
  void update(int value) {
    Serial.print("Received update: ");
    Serial.println(value);
  }
};

void setup() {
  Serial.begin(9600);

  Subject subject;
  ConcreteObserver observer1;
  ConcreteObserver observer2;
  ConcreteObserver observer3;

  subject.attach(&observer1);
  subject.attach(&observer2);
  subject.attach(&observer3);

  subject.notify(123);

  subject.detach(&observer2);

  subject.notify(456);
}

void loop() {
  // Nothing to do here
}
