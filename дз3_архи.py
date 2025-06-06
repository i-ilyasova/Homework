import mmap
import os
from multiprocessing import Process

def producer(buffer_file, size=100):
    with open(buffer_file, 'wb+') as f:
        # Создаём файл нужного размера и записываем нули
        f.write(b'\0' * size)
        f.flush()
        with mmap.mmap(f.fileno(), size, access=mmap.ACCESS_WRITE) as m:
            message = b"Hello from Producer to Consumer via mmap!"
            m[:len(message)] = message
            print("Производитель отправил сообщение")

def consumer(buffer_file, size=100):
    with open(buffer_file, 'rb') as f:
        with mmap.mmap(f.fileno(), size, access=mmap.ACCESS_READ) as m:
            data = m.read(size)
            # Находим конец строки (до первого нуля или конца)
            end = data.find(b'\0')
            if end != -1:
                print("Потребитель получил:", data[:end].decode('utf-8'))
            else:
                print("Потребитель получил:", data.decode('utf-8'))

if __name__ == '__main__':
    buffer_file = "shared_buffer.dat"
    size = 100

    # Запуск процессов
    p_producer = Process(target=producer, args=(buffer_file, size))
    p_consumer = Process(target=consumer, args=(buffer_file, size))

    p_producer.start()
    p_producer.join()  # Ждём, пока производитель запишет данные
    p_consumer.start()
    p_consumer.join()

    # Удаление временного файла
    os.unlink(buffer_file)
