import argparse
import sys
import os

class DependencyVisualizer:
    
    def __init__(self):
        self.package_name = None
        self.repository_source = None
        self.test_mode = False
        self.parameters = {}
    
    def parse_arguments(self):
        parser = argparse.ArgumentParser(
            description='Инструмент визуализации графа зависимостей для менеджера пакетов',
            formatter_class=argparse.RawDescriptionHelpFormatter,
            epilog="""
Примеры использования:
  %(prog)s --package express --source https://registry.npmjs.org
  %(prog)s --package myapp --source ./test_repo.json --test-mode
            """
        )
        
        parser.add_argument(
            '--package',
            type=str,
            required=True,
            help='Имя анализируемого пакета (например: express, react)'
        )
        
        parser.add_argument(
            '--source',
            type=str,
            required=True,
            help='URL-адрес репозитория или путь к файлу тестового репозитория'
        )
        
        parser.add_argument(
            '--test-mode',
            action='store_true',
            default=False,
            help='Режим работы с тестовым репозиторием'
        )
        
        return parser.parse_args()
    
    def validate_arguments(self, args):
        errors = []
        
        if not args.package or args.package.strip() == '':
            errors.append("Имя пакета не может быть пустым")
        elif len(args.package) > 100:
            errors.append("Имя пакета слишком длинное")
        
        if not args.source or args.source.strip() == '':
            errors.append("Источник данных не может быть пустым")
        
        if args.test_mode:
            if not os.path.exists(args.source):
                errors.append(f"Тестовый файл '{args.source}' не найден")
            elif not args.source.endswith('.json'):
                errors.append(f"Тестовый файл должен быть в формате JSON. Получено: {args.source}")
        else:
            if not (args.source.startswith('http://') or args.source.startswith('https://')):
                errors.append(f"Источник должен быть валидным URL. Получено: {args.source}")
        
        if errors:
            error_message = "\n".join([f"  • {error}" for error in errors])
            raise ValueError(f"Ошибки в параметрах:\n{error_message}")
    
    def save_parameters(self, args):
        self.parameters = {
            'Имя анализируемого пакета': args.package,
            'Источник данных': args.source,
            'Режим работы с тестовым репозиторием': 'Да' if args.test_mode else 'Нет'
        }
    
    def print_parameters(self):
        print("=" * 50)
        print("ПАРАМЕТРЫ КОНФИГУРАЦИИ:")
        print("=" * 50)
        
        for key, value in self.parameters.items():
            print(f"{key:35} : {value}")
        
        print("=" * 50)
    
    def run(self):
        try:
            print(" Парсинг аргументов командной строки...")
            args = self.parse_arguments()
            
            print("Валидация параметров...")
            self.validate_arguments(args)
            
            print("Сохранение параметров...")
            self.save_parameters(args)
            
            self.print_parameters()
            
            print("Все параметры корректны! Приложение готово к работе.")
            print(f"\nСледующие шаги:")
            print(f"  1. Анализ пакета: {args.package}")
            print(f"  2. Использование источника: {args.source}")
            print(f"  3. Режим: {'тестовый' if args.test_mode else 'рабочий'}")
            
            return 0
            
        except argparse.ArgumentError as e:
            print(f"Ошибка в аргументах командной строки: {e}")
            print("Используйте --help для справки по параметрам")
            return 1
            
        except ValueError as e:
            print(f"Ошибка валидации параметров:\n{e}")
            return 2
            
        except KeyboardInterrupt:
            print("Выполнение прервано пользователем")
            return 130
            
        except Exception as e:
            print(f"Непредвиденная ошибка: {e}")
            print("Пожалуйста, сообщите об этой ошибке разработчикам")
            return 3

def main():
    visualizer = DependencyVisualizer()
    exit_code = visualizer.run()
    sys.exit(exit_code)

if __name__ == "__main__":
    main()
