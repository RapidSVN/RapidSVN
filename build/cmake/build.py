import subprocess
print("Configure:")
subprocess.run(["cmake", "-H.", "-B_build"], check=True)
print("Build:")
subprocess.run(["cmake", "--build", "_build",
                "--config", "RelWithDebInfo"], check=True)
