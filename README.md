# RootCDH

This repository is for the ASEN Lunar Greenhouse Project 26/27 Root Team.

## Git Usage

All new hardware should be implemented in their own branch.

```bash
git branch My_Branch_Name
git checkout My_Branch_Name
```

Once your changes are committed and ready for review, push your branch to the remote repository:

```bash
git push
```

Then create a pull request on GitHub.

## Repository Structure

All third-party libraries that we bring into the repo will be put into the components/ directory in their own folder. This does not include dependencies that are included by the esp idf component manager which will be placed automatically into managed_components/. For example the Senseair Sunrise code is in components/sensair_sunrise/sensair_sunrise.hpp.

Keep application-specific code under main/. Place sensor interfaces and implementations under main/sensors/, and place control logic under main/controls/ as it is added. Keep main.cpp focused on application initialization and task startup.
