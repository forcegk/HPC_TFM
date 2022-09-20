def fn_algebra_ops(args):
    idx, local_layer_lud, MAP_ALGORITHM = args

    local_algebra_ops = [f"    fprintf(stderr, \"*** POINT-TO-POINT MATMUL %s x %s ***\\n\", \"layer{idx-1}\", \"layer{idx}\");"]

    local_p2p_ops = []
    for x,y in local_layer_lud.keys():
        local_p2p_ops.append(f"        temp_output[{x}].val += layer{idx}_weights[{local_layer_lud[(x,y)]}].val * temp_input[{y}].val;\n")

    # weird indentation to make it look pretty (nothing else :)
    local_algebra_ops.append(f"""
    for(int k = 0; k < input_dim; k++){{
        temp_input = layer{idx-1}_out + k*LAYER{idx-1}_SIZE;
        temp_output = layer{idx}_out + k*LAYER{idx}_SIZE;\n\n{"".join(local_p2p_ops)}    }}
    """)

    # Una optimización muy sencilla es meter el free de layer{idx-1} después del sgemm, pero debido a que esto es una poc, no voy a complicarme ahora mismo, a no ser que sea necesario. 
    local_algebra_ops.append(f"fprintf(stderr, \"*** Map and Bias %s(T) with %s function ***\\n\", \"layer{idx}_out\", \"{MAP_ALGORITHM}\");\n")
    local_algebra_ops.append(f"    map_and_bias__fp32(layer{idx}_out, layer{idx}_bias, input_dim, LAYER{idx}_SIZE, 'N', {MAP_ALGORITHM});\n\n")

    return "".join(local_algebra_ops)