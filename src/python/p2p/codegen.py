def fn_algebra_ops(args):
    idx, local_layer_lud, MAP_ALGORITHM = args

    local_algebra_ops = [f"#ifdef VERBOSE\n#pragma omp master\n{{\n    fprintf(stderr, \"*** [\"BOLDGREEN\"TIME\"RESET\"] POINT-TO-POINT MATMUL %s x %s \", \"layer{idx-1}\", \"layer{idx}\");\n    clock_gettime(CLOCK_MONOTONIC_RAW, &time_start);\n}}\n#endif"]

    local_p2p_ops = []
    for x,y in local_layer_lud.keys():
        local_p2p_ops.append(f"        temp_output[{x}].val += layer{idx}_weights[{local_layer_lud[(x,y)]}].val * temp_input[{y}].val;\n")

    # weird indentation to make it look pretty (nothing else :)
    local_algebra_ops.append(f"""
#pragma omp for
    for(int k = 0; k < input_dim; k++){{
        temp_input = layer{idx-1}_out + k*LAYER{idx-1}_SIZE;
        temp_output = layer{idx}_out + k*LAYER{idx}_SIZE;\n\n{"".join(local_p2p_ops)}    }}
""")

    # Una optimización muy sencilla es meter el free de layer{idx-1} después del sgemm, pero debido a que esto es una poc, no voy a complicarme ahora mismo, a no ser que sea necesario. 
    local_algebra_ops.append(f"#ifdef VERBOSE\n#pragma omp master\n{{\n    clock_gettime(CLOCK_MONOTONIC_RAW, &time_end);\n    elapsed_us = (time_end.tv_sec - time_start.tv_sec) * 1000000 + (time_end.tv_nsec - time_start.tv_nsec) / 1000;\n    delta_gemm_us += elapsed_us;\n    fprintf(stderr, \"took %d µs ***\\n\", elapsed_us);\n    fprintf(stderr, \"*** [\"BOLDGREEN\"TIME\"RESET\"] Map and Bias %s(T) with %s function \", \"layer{idx}_out\", \"{MAP_ALGORITHM}\");\n    clock_gettime(CLOCK_MONOTONIC_RAW, &time_start);\n}}\n#endif\n")
    local_algebra_ops.append(f"    map_and_bias__fp32(layer{idx}_out+start*LAYER{idx}_SIZE, layer{idx}_bias, end-start, LAYER{idx}_SIZE, 'N', {MAP_ALGORITHM});\n#pragma omp barrier\n")
    local_algebra_ops.append(f"#ifdef VERBOSE\n#pragma omp master\n{{\n    clock_gettime(CLOCK_MONOTONIC_RAW, &time_end);\n    elapsed_us = (time_end.tv_sec - time_start.tv_sec) * 1000000 + (time_end.tv_nsec - time_start.tv_nsec) / 1000;\n    delta_map_bias_us += elapsed_us;\n    fprintf(stderr, \"took %d µs ***\\n\", elapsed_us);\n}}\n#endif\n\n")

    return "".join(local_algebra_ops)