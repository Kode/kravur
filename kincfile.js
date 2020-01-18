var project = new Project('kravur');

project.setCmd();
project.kore = false;

project.addExclude(".git/**");
project.addExclude("build/**");

project.addFile('Sources/**');

resolve(project);
