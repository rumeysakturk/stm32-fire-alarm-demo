CC ?= cc
BUILD_DIR ?= build
TARGET := $(BUILD_DIR)/fire_alarm_host_demo
SOURCES := Core/Src/fire_alarm.c host_demo/src/fake_hal.c host_demo/src/main.c

CPPFLAGS += -Ihost_demo/include -ICore/Inc
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic

.PHONY: all demo clean

all: $(TARGET)

demo: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCES) Core/Inc/fire_alarm.h Core/Inc/main.h host_demo/include/stm32f4xx_hal.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
