import sys
from src.cli import run_cli # type: ignore


def main() -> int:
    success = run_cli()
    
    if success:
        return 0
    else:
        return 1


if __name__ == "__main__":
    sys.exit(main())
