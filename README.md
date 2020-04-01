# AD9910-DUE
Arduino code for translating Cicero (AMO physics software) commands to update DDS registers.

Designed for use with the Arduino DUE controlling several AD9910 devices. Compile, test, edit in the usual Arduino IDE.

The main file is Due_4_1_threeDDS.ino which includes the setup and loop functions. DDS_math.ino includes some user defined operations (e.g. calculating FTW). Interp_C.ino is the main piece for interpreting Cicero commands for the AD9910. Finally, Profile_upload.ino and RAM_upload.ino include code for uploading to the single-tone profile and RAM registers (although RAM upload was never fully developed).


For questions about either software or hardware configuration, contact dlg15@uw.edu or thegochnauer@gmail.com.
