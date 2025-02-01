# Open Me Daily App C++ Project

This project logs and tracks application openings using SQLite. It consists of two primary components:

- **Application Start Logging**: Tracks the number of times the application has been opened each day.
- **Password Entry Logging**: Marks whether the user has entered a password during the application's usage.

## Workflow

- When the computer starts and the application is launched, it logs the application opening.
- If the user doesn't enter the password, the system's disk usage will be filled up as part of the application logic.
- If the user enters the correct phrase ("ben buradayım ve mutluyum") when running the application, it marks the password as entered for the day.

## Database

The database (`log.db`) contains a table `log` with the following fields:

- **`date`**: The date of the application launch.
- **`boot_count`**: The number of times the application has been launched on that date.
- **`is_password_entered`**: Marks whether the password has been entered for the current day.

## How it Works

1. **On Application Start**: The application checks if the user entered the correct phrase to mark the password as entered.
2. **Password Entry**: If the correct phrase is provided, the password entry is logged.
3. **No Password**: If the password is not entered, the system will fill up its disks as part of the logic.

## Usage

To indicate password entry, run the program with the correct phrase: `"ben buradayım ve mutluyum"`. For example, in a Bash terminal:

```bash
    ./application "ben buradayım ve mutluyum"
```

If the correct phrase is entered, the application will log that the password was entered for the day.

If the correct phrase is not entered, the program will simply log the application startup, as shown below:

``` bash
    ./application
```

In this case, the application will log the startup but won't mark the password as entered.

For interested people I add the input of:

``` bash
    ./application "my logs"
```

to see the logs which program has about the user

## Dependencies

- SQLite3 library for C++ (`sqlite3.h`). I used `vcpkg` on here.
- Standard C++ libraries.

## License

This project is open-source. Feel free to contribute or modify as needed.
