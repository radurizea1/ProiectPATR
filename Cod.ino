#include <Arduino_FreeRTOS.h> // Este inclusa biblioteca FreeRTOS
#include <semphr.h> // Este inclusa biblioteca semhr.h, pentru utilizarea mutexurilor

SemaphoreHandle_t mutex;
SemaphoreHandle_t mutex2;
int ok;
void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT); //TASK_A
  pinMode(3, OUTPUT); //TASK_B
  pinMode(4, OUTPUT); //TASK_C
  Serial.begin(9600); // Este initializata comunicatia seriala
  ok = 0;
  Serial.println("OK este 0");
  mutex = xSemaphoreCreateMutex();

  // Sunt create cele 3 taskuri, fiecare cu 128 de cuvinte de memorie rezervate si prioritate egala cu 1
  xTaskCreate(TASK_A, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TASK_B, "task2", 128, NULL, 1, NULL);
  xTaskCreate(TASK_C, "task3", 128, NULL, 1, NULL);
  vTaskStartScheduler();
}

int i = 20;

void TASK_A(){
  while(1){
    if(i < 200){
      xSemaphoreTake(mutex, portMAX_DELAY);
        
      for(i = 190; i < 200; i++){
          digitalWrite(2, HIGH);
          vTaskDelay(200 / portTICK_PERIOD_MS);
          Serial.println(i);
          digitalWrite(2, LOW);
          vTaskDelay(200 / portTICK_PERIOD_MS);
      }
      xSemaphoreGive(mutex);
      digitalWrite(2, HIGH);
    }
    else{
      ok = 1;
      Serial.println(i);
      digitalWrite(2, LOW);
    }
    
    vTaskDelay(200 / portTICK_PERIOD_MS);
    
  }
}
void TASK_C(){
 
    while(1){
        Serial.println("citeste OK");
       if(ok == 1){
        xSemaphoreTake(mutex, portMAX_DELAY);
        Serial.println("Interpreteaza Gcode-ul");
        digitalWrite(4, HIGH);
        delay(800);
        Serial.println(i);
        digitalWrite(4, LOW);
        delay(800);
        ok = 1;
        TASK_B();
    }
  }
}

void TASK_B(){
  
    while(1){
      if(ok == 1){
        Serial.println("Citeste informatia de la optocuplor");
        digitalWrite(3, HIGH);
        delay(400);
        Serial.println(i);
        digitalWrite(3, LOW);
        delay(400);
        ok = 1;
        xSemaphoreGive(mutex);
        TASK_C();
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  
}
